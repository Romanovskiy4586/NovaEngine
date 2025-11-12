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
			_gameWindow.baseWindow.SubscribeOnFramebuffer([this](int width, int height) { _FramebufferSizeCallback(width, height); });

			_InitGame();
			_SetupEngine();
			_InitCamera();
			_LoadTextures();
			_CreateSprites();
			_SetSpritesDepths();
			_CreateSSBOs();
			_UpdateSSBOs();
		}
		void _Update(double delta) NSL_NOEXCEPT
		{
			_HandleInput(delta);
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
			//_gameWindow.renderHandler.RenderSprite("Atlas", _camera);
			//_gameWindow.renderHandler.RenderSpriteInstanced("EmptyTile", _camera, "EmptyTilesPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("WallTile", _camera, "WallTilesPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("LowtierStalker", _camera, "LowtierStalkersPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("MidlowtierStalker", _camera, "MidlowtierStalkersPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("MidtierStalker", _camera, "MidtierStalkersPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("MidhightierStalker", _camera, "MidhightierStalkersPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("HightierStalker", _camera, "HightierStalkersPositions");
			//_gameWindow.renderHandler.RenderSpriteInstanced("Point", _camera, "Astar");

			//_gameWindow.renderHandler.GetSprite("Atlas").transform.SetPosition(NSL::Vector3(0.0f, 0.0f, 0.0f));
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "EmptyTilesPositions", atlasSize, tileSize, emptyTileIndex);
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "WallTilesPositions", atlasSize, tileSize, wallTileIndex);
			//_gameWindow.renderHandler.GetSprite("Atlas").transform.SetPosition(NSL::Vector3(0.0f, 0.0f, 0.002f));
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "LowtierStalkersPositions", atlasSize, tileSize, lowtierStalkerIndex);
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "MidlowtierStalkersPositions", atlasSize, tileSize, midlowtierStalkerIndex);
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "MidtierStalkersPositions", atlasSize, tileSize, midtierStalkerIndex);
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "MidhightierStalkersPositions", atlasSize, tileSize, midhightierStalkerIndex);
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "HightierStalkersPositions", atlasSize, tileSize, hightierStalkerIndex);
			//_gameWindow.renderHandler.GetSprite("Atlas").transform.SetPosition(NSL::Vector3(0.0f, 0.0f, 0.001f));
			//_gameWindow.renderHandler.RenderSpriteInstancedSampled("Atlas", _camera, "Astar", atlasSize, tileSize, pointIndex);
			_gameWindow.renderHandler.RenderText(_camera, "Hello, World!");
		}
		void _Unload() NSL_NOEXCEPT
		{
			_UnloadTextures();
			_UnloadSSBOs();
		}

		void _FramebufferSizeCallback(int width, int height) NSL_NOEXCEPT
		{
			_gameWindow.renderHandler.SetViewport(width, height);
			_camera.SetWidthAndHeight(width, height);
		}

		void _InitGame() NSL_NOEXCEPT
		{
			NSL::JSON json = NSL::JSON::Load("StalkerConfig.json");
			const int WALLS_COUNT = static_cast<int>(json.root.Int("numWalls"));
			const int MAP_WIDTH =	static_cast<int>(json.root.Int("mapWidth"));
			const int MAP_HEIGHT =	static_cast<int>(json.root.Int("mapHeight"));

			_world.RebuildMap(MAP_WIDTH, MAP_HEIGHT);

			NSL::Vector2ui position;
			for (int i = 0; i < WALLS_COUNT; ++i)
			{
				do
				{
					position.x = NSL::Random(0, MAP_WIDTH - 1);
					position.y = NSL::Random(0, MAP_HEIGHT - 1);
				}
				while (_world.GetTile(position.x, position.y).type == Tile::Type::Wall);

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
		void _SetupEngine() NSL_NOEXCEPT
		{
			_gameWindow.baseWindow.UI.LoadFont("Assets/Fonts/CascadiaMono.ttf", _gameWindow.baseWindow.monitor.GetPPM() * 4.0f);
			_gameWindow.baseWindow.renderWindowState = true;
			_gameWindow.renderHandler.SetClearColor(NSL::Vector4(0.5f, 0.9f, 1.0f, 1.0f));
		}
		void _InitCamera() NSL_NOEXCEPT
		{
			NSL::JSON json = NSL::JSON::Load("StalkerConfig.json");
			const int MAP_WIDTH =  static_cast<int>(json.root.Int("mapWidth"));
			const int MAP_HEIGHT = static_cast<int>(json.root.Int("mapHeight"));

			_camera.SetWidthAndHeight(_gameWindow.baseWindow.GetFramebufferWidth(), _gameWindow.baseWindow.GetFramebufferHeight());
			_camera.SetFOVClampingValue(NSL::Vector2(250.0f, 100000.0f));
			_camera.SetFOV(29.0f * MAP_WIDTH);
			_camera.SetProjectionType(Renderer::Camera::ProjectionType::Orthographic);
			_camera.SetDepthNear(-1000.0f);
			_camera.SetPosition(NSL::Vector3(float(MAP_WIDTH) / 2, float(MAP_HEIGHT) / 2, 0.0f));
		}
		void _LoadTextures() NSL_NOEXCEPT
		{
			//_gameWindow.renderHandler.LoadTexture2D("Font", "Assets/STALKER/Font.png");
			//_gameWindow.renderHandler.LoadTexture2D("EmptyTile", "Assets/STALKER/EmptyTile.png");
			//_gameWindow.renderHandler.LoadTexture2D("WallTile", "Assets/STALKER/WallTile.png");
			//_gameWindow.renderHandler.LoadTexture2D("LowtierStalker", "Assets/STALKER/LowtierStalker.png");
			//_gameWindow.renderHandler.LoadTexture2D("MidlowtierStalker", "Assets/STALKER/MidlowtierStalker.png");
			//_gameWindow.renderHandler.LoadTexture2D("MidtierStalker", "Assets/STALKER/MidtierStalker.png");
			//_gameWindow.renderHandler.LoadTexture2D("MidhightierStalker", "Assets/STALKER/MidhightierStalker.png");
			//_gameWindow.renderHandler.LoadTexture2D("HightierStalker", "Assets/STALKER/HightierStalker.png");
			//_gameWindow.renderHandler.LoadTexture2D("Point", "Assets/STALKER/Point.png");
			_gameWindow.renderHandler.LoadTexture2D("TextureAtlas", "Assets/STALKER/TextureAtlas.png", Renderer::Texture2DFiltering::Nearest);
		}
		void _CreateSprites() NSL_NOEXCEPT
		{
			//_gameWindow.renderHandler.CreateSprite("EmptyTile", "EmptyTile");
			//_gameWindow.renderHandler.CreateSprite("WallTile", "WallTile");
			//_gameWindow.renderHandler.CreateSprite("LowtierStalker", "LowtierStalker");
			//_gameWindow.renderHandler.CreateSprite("MidlowtierStalker", "MidlowtierStalker");
			//_gameWindow.renderHandler.CreateSprite("MidtierStalker", "MidtierStalker");
			//_gameWindow.renderHandler.CreateSprite("MidhightierStalker", "MidhightierStalker");
			//_gameWindow.renderHandler.CreateSprite("HightierStalker", "HightierStalker");
			//_gameWindow.renderHandler.CreateSprite("Point", "Point");
			_gameWindow.renderHandler.CreateSprite("Atlas", "TextureAtlas");
		}
		void _SetSpritesDepths() NSL_NOEXCEPT
		{
			//NSL::Vector3 position0(0.0f, 0.0f, 0.0f);
			//NSL::Vector3 position1(0.0f, 0.0f, 0.001f);
			//NSL::Vector3 position2(0.0f, 0.0f, 0.002f);

			//_gameWindow.renderHandler.GetSprite("EmptyTile").transform.SetPosition(position0);
			//_gameWindow.renderHandler.GetSprite("WallTile").transform.SetPosition(position0);
			//_gameWindow.renderHandler.GetSprite("LowtierStalker").transform.SetPosition(position1);
			//_gameWindow.renderHandler.GetSprite("MidlowtierStalker").transform.SetPosition(position1);
			//_gameWindow.renderHandler.GetSprite("MidtierStalker").transform.SetPosition(position1);
			//_gameWindow.renderHandler.GetSprite("MidhightierStalker").transform.SetPosition(position1);
			//_gameWindow.renderHandler.GetSprite("HightierStalker").transform.SetPosition(position1);
			//_gameWindow.renderHandler.GetSprite("Point").transform.SetPosition(position1);
		}
		void _CreateSSBOs() NSL_NOEXCEPT
		{
			_gameWindow.renderHandler.CreateShaderStorageBuffer("EmptyTilesPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("WallTilesPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("LowtierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("MidlowtierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("MidtierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("MidhightierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("HightierStalkersPositions");
			_gameWindow.renderHandler.CreateShaderStorageBuffer("Astar");
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

		void _UnloadTextures() NSL_NOEXCEPT
		{
			//_gameWindow.renderHandler.DeleteTexture2D("Font");
			//_gameWindow.renderHandler.DeleteTexture2D("EmptyTile");
			//_gameWindow.renderHandler.DeleteTexture2D("WallTile");
			//_gameWindow.renderHandler.DeleteTexture2D("LowtierStalker");
			//_gameWindow.renderHandler.DeleteTexture2D("MidlowtierStalker");
			//_gameWindow.renderHandler.DeleteTexture2D("MidtierStalker");
			//_gameWindow.renderHandler.DeleteTexture2D("MidhightierStalker");
			//_gameWindow.renderHandler.DeleteTexture2D("HightierStalker");
			//_gameWindow.renderHandler.DeleteTexture2D("Point");
			_gameWindow.renderHandler.DeleteTexture2D("TextureAtlas");
		}
		void _UnloadSSBOs() NSL_NOEXCEPT
		{
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("EmptyTilesPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("WallTilesPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("LowtierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("MidlowtierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("MidtierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("MidhightierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("HightierStalkersPositions");
			_gameWindow.renderHandler.DeleteShaderStorageBuffer("Astar");
		}

		void _HandleInput(double delta) NSL_NOEXCEPT
		{
			static float speed;
			static bool isNeedToUpdateSSBOs = false;
			static bool isRebuildingMap = false;
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
					} while (_path.empty());
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

	private:
		Engine::GameWindow _gameWindow;
		StalkerWorld _world;
		Renderer::Camera _camera;
		std::vector<NSL::Vector2> _path;
	};
} 