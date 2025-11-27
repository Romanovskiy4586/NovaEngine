export module StalkerGame;
#include "Core.h"
import std;
import NovaEngine;
import StalkerWorld;

export namespace Stalker
{
	class StalkerGame final
	{
	public:
		StalkerGame() NSL_NOEXCEPT
		{
			_gameWindow.load =	 [this]() { _Load(); };
			_gameWindow.update = [this](double delta) { _Update(delta); };
			_gameWindow.render = [this](double delta) { _Render(delta); };
			_gameWindow.unload = [this]() { _Unload(); };
		}

		void Run() NSL_NOEXCEPT
		{
			_gameWindow.Run();
		}

	private:
		void _Load() NSL_NOEXCEPT
		{
			_gameWindow.baseWindow.SubscribeOnFramebuffer([this](int width, int height)
			{
				_gameWindow.renderHandler.SetViewport(width, height);
				_camera.SetWidthAndHeight(width, height);
			});

			// -------------------------------------------------- LOAD GAME -------------------------------------------------- //
			_InitGame();

			// -------------------------------------------------- SETUP ENGINE -------------------------------------------------- //
			_gameWindow.baseWindow.UI.LoadFont("Assets/Fonts/CascadiaMono.ttf", _gameWindow.baseWindow.monitor.GetPPM() * 4.0f);
			_gameWindow.baseWindow.renderWindowState = true;
			//_gameWindow.renderHandler.SetClearColor(NSL::Vector4(0.5f, 0.9f, 1.0f, 1.0f));
			_gameWindow.renderHandler.SetClearColor(NSL::Vector4(0.3f, 0.3f, 0.3f, 1.0f));

			// -------------------------------------------------- INIT CAMERA -------------------------------------------------- //
			NSL::JSON stalkerConfigJson = NSL::JSON::Load("StalkerConfig.json");
			const int MAP_WIDTH = static_cast<int>(stalkerConfigJson.root.Int("mapWidth"));
			const int MAP_HEIGHT = static_cast<int>(stalkerConfigJson.root.Int("mapHeight"));
			_camera.SetWidthAndHeight(_gameWindow.baseWindow.GetFramebufferWidth(), _gameWindow.baseWindow.GetFramebufferHeight());
			_camera.SetFOVClampingValue(NSL::Vector2(250.0f, 100000.0f));
			_camera.SetFOV(29.0f * MAP_WIDTH);
			_camera.SetProjectionType(Renderer::Camera::ProjectionType::Orthographic);
			_camera.SetDepthNear(-1000.0f);
			_camera.SetPosition(NSL::Vector3(float(MAP_WIDTH) / 2, float(MAP_HEIGHT) / 2, 0.0f));
			//_camera = Renderer::Camera(_gameWindow.baseWindow.GetFramebufferWidth(), _gameWindow.baseWindow.GetFramebufferHeight());

			// -------------------------------------------------- SETUP RESOURCES -------------------------------------------------- //
			// TEXTURES
			_gameWindow.renderHandler.LoadTexture2D("TextureAtlas", "Assets/STALKER/TextureAtlas.png", Renderer::Texture2DFiltering::Nearest);
			// SPRITES
			_gameWindow.renderHandler.CreateSprite("Atlas", "TextureAtlas");
			// SSBOs
			_gameWindow.renderHandler.CreateShaderStorageBuffer("EmptyTilesPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("WallTilesPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("LowtierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("MidlowtierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("MidtierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("MidhightierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("HightierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("Astar");

			_UpdateSSBOs();
		}

		void _Update(double delta) NSL_NOEXCEPT
		{
			static float speed;
			static bool isNeedToUpdateSSBOs = false;
			static bool isRebuildingMap = false;
			static NSL::JSON stalkerConfigJson = NSL::JSON::Load("StalkerConfig.json");
			static const int PATH_SIZE = static_cast<int>(stalkerConfigJson.root.Int("pathSize"));

			speed = 0.00002f * static_cast<float>(delta) * _camera.GetFOV();

			_camera.AddToFOV(-_gameWindow.baseWindow.GetMouseScrollDelta() * _camera.GetFOV() * 0.1f);

			if (_gameWindow.baseWindow.IsKeyboardKeyDown(IO::KeyboardKey::W))
			{
				_camera.AddToPosition(NSL::Vector3::UnitY * speed);
			}
			if (_gameWindow.baseWindow.IsKeyboardKeyDown(IO::KeyboardKey::S))
			{
				_camera.AddToPosition(-NSL::Vector3::UnitY * speed);
			}
			if (_gameWindow.baseWindow.IsKeyboardKeyDown(IO::KeyboardKey::A))
			{
				_camera.AddToPosition(-NSL::Vector3::UnitX * speed);
			}
			if (_gameWindow.baseWindow.IsKeyboardKeyDown(IO::KeyboardKey::D))
			{
				_camera.AddToPosition(NSL::Vector3::UnitX * speed);
			}
			if (_gameWindow.baseWindow.IsKeyboardKeyDown(IO::KeyboardKey::Q))
			{
				_camera.AddToPosition(NSL::Vector3::UnitZ * speed);
			}
			if (_gameWindow.baseWindow.IsKeyboardKeyDown(IO::KeyboardKey::E))
			{
				_camera.AddToPosition(-NSL::Vector3::UnitZ * speed);
			}
			if (_gameWindow.baseWindow.IsMouseButtonDown(IO::MouseButton::Middle))
			{
				_camera.InputMouseDelta(_gameWindow.baseWindow.GetCursorPositionDelta().x, _gameWindow.baseWindow.GetCursorPositionDelta().y);
			}
			if (_gameWindow.baseWindow.IsKeyboardKeyPressed(IO::KeyboardKey::Space) && !isRebuildingMap)
			{
				isRebuildingMap = true;
				std::thread thrd([this]()
					{
						do
						{
							_InitGame();

						} while (_path.size() < PATH_SIZE);
						isNeedToUpdateSSBOs = true;
						isRebuildingMap = false;
					});
				thrd.detach();
			}
			if (isNeedToUpdateSSBOs)
			{
				_UpdateSSBOs();
				isNeedToUpdateSSBOs = false;
			}
		}

		void _Render(double delta) NSL_NOEXCEPT
		{
			static const NSL::Vector2 atlasSize(256);
			static const NSL::Vector2 tileSize(16);

			static const NSL::Vector2 emptyTileIndex(0.0f, 0.0f);
			static const NSL::Vector2 wallTileIndex(1.0f, 0.0f);
			static const NSL::Vector2 pointIndex(2.0f, 0.0f);

			static const NSL::Vector2 lowtierStalkerIndex(0.0f, 1.0f);
			static const NSL::Vector2 midlowtierStalkerIndex(1.0f, 1.0f);
			static const NSL::Vector2 midtierStalkerIndex(2.0f, 1.0f);
			static const NSL::Vector2 midhightierStalkerIndex(3.0f, 1.0f);
			static const NSL::Vector2 hightierStalkerIndex(4.0f, 1.0f);

			_gameWindow.renderHandler.Clear();

			_gameWindow.renderHandler.GetSprite("Atlas").transform.SetPosition(NSL::Vector3(0.0f, 0.0f, 0.0f));
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "EmptyTilesPositions", atlasSize, tileSize, emptyTileIndex);
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "WallTilesPositions", atlasSize, tileSize, wallTileIndex);
			_gameWindow.renderHandler.GetSprite("Atlas").transform.SetPosition(NSL::Vector3(0.0f, 0.0f, 0.02f));
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "LowtierStalkersPositions", atlasSize, tileSize, lowtierStalkerIndex);
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "MidlowtierStalkersPositions", atlasSize, tileSize, midlowtierStalkerIndex);
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "MidtierStalkersPositions", atlasSize, tileSize, midtierStalkerIndex);
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "MidhightierStalkersPositions", atlasSize, tileSize, midhightierStalkerIndex);
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "HightierStalkersPositions", atlasSize, tileSize, hightierStalkerIndex);
			_gameWindow.renderHandler.GetSprite("Atlas").transform.SetPosition(NSL::Vector3(0.0f, 0.0f, 0.01f));
			_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "Astar", atlasSize, tileSize, pointIndex);

			static int i = 0;
			static NSL::Vector3 pos;

			pos = NSL::Vector3::Zero;
			pos.y -= 4.0f;
			_gameWindow.renderHandler.RenderText(_camera, "Hello, World! This is a long text with different symbols: " + std::to_string(i++), pos);
			pos.y -= 2.0f;
			_gameWindow.renderHandler.RenderText(_camera, "I uses the same SSBO as the last time, in a line at the top", pos);
			pos.y -= 2.0f;
			_gameWindow.renderHandler.RenderText(_camera, "This line uses the same SSBO, i'm trying to (load) {it}", pos);	
		}

		void _Unload() NSL_NOEXCEPT
		{
			// TEXTURES
			_gameWindow.renderHandler.DeleteTexture2D("TextureAtlas");

			// SSBOs
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("EmptyTilesPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("WallTilesPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("LowtierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("MidlowtierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("MidtierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("MidhightierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("HightierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("Astar");
		}

		void _UpdateSSBOs() NSL_NOEXCEPT
		{
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("EmptyTilesPositions", _world.GetEmptyTilesPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("WallTilesPositions", _world.GetWallTilesPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("LowtierStalkersPositions", _world.GetLowtierStalkersPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("MidlowtierStalkersPositions", _world.GetMidlowtierStalkersPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("MidtierStalkersPositions", _world.GetMidtierStalkersPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("MidhightierStalkersPositions", _world.GetMidhightierStalkersPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("HightierStalkersPositions", _world.GetHightierStalkersPositions());
			_gameWindow.renderHandler.UpdateShaderStorageBuffer("Astar", _path);
		}
		void _InitGame() NSL_NOEXCEPT
		{
			static NSL::JSON stalkerConfigJson = NSL::JSON::Load("StalkerConfig.json");
			static const int WALLS_COUNT = static_cast<int>(stalkerConfigJson.root.Int("numWalls"));
			static const int MAP_WIDTH = static_cast<int>(stalkerConfigJson.root.Int("mapWidth"));
			static const int MAP_HEIGHT = static_cast<int>(stalkerConfigJson.root.Int("mapHeight"));

			_world.RebuildMap(MAP_WIDTH, MAP_HEIGHT);
			NSL::Vector2ui position;
			for (int i = 0; i < WALLS_COUNT; ++i)
			{
				do
				{
					position.x = NSL::Random(0, MAP_WIDTH - 1);
					position.y = NSL::Random(0, MAP_HEIGHT - 1);

				} while (_world.GetTile(position.x, position.y).type == Tile::Type::Wall);

				_world.SetTileType(position.x, position.y, Tile::Type::Wall);
			}
			_world.SetTileType(0, 0, Tile::Type::Empty);
			_world.SetTileType(MAP_WIDTH - 1, MAP_HEIGHT - 1, Tile::Type::Empty);
			_world.SpawnNPC(Stalker::NPC::Type::LowtierStalker, NSL::Vector2ui::Zero);
			auto path = _world.Astar2(NSL::Vector2ui::Zero, NSL::Vector2ui(MAP_WIDTH - 1, MAP_HEIGHT - 1));
			_path.resize(path.size());
			for (size_t i = 0; i < path.size(); ++i)
			{
				_path[i].x = static_cast<float>(path[i].x);
				_path[i].y = static_cast<float>(path[i].y);
			}
		}
	private:
		Engine::GameWindow _gameWindow;
		StalkerWorld _world;
		Renderer::Camera _camera;
		std::vector<NSL::Vector2> _path;
	};
} 