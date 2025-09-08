export module BugsWindow;

import NovaEngine;
export import World;

#include "Core.h"

NSL::Vector2 instances;

export namespace Bugs
{
	class BugsWindow : public Engine::GameWindow
	{
	public:
		virtual ~BugsWindow() = default;

	private:
		virtual void Load() NSL_NOEXCEPT override
		{
			// ------------------------ Инициализация игрового контекста ----------------------- //
			NSL::JSON json2 = NSL::JSON::Load("RenderParameters.json");
			NSL::JSON json = NSL::JSON::Load("BugsConfig.json");

			instances.x = json2.root.Array("instances").Int(0);
			instances.y = json2.root.Array("instances").Int(1);

			_emptyColor = NSL::Color4
			(
				json.root.Array("emptyColor").Int(0),
				json.root.Array("emptyColor").Int(1),
				json.root.Array("emptyColor").Int(2),
				json.root.Array("emptyColor").Int(3)
			);
			_wallColor = NSL::Color4
			(
				json.root.Array("wallColor").Int(0),
				json.root.Array("wallColor").Int(1),
				json.root.Array("wallColor").Int(2),
				json.root.Array("wallColor").Int(3)
			);
			_bugColor = NSL::Color4
			(
				json.root.Array("bugColor").Int(0),
				json.root.Array("bugColor").Int(1),
				json.root.Array("bugColor").Int(2),
				json.root.Array("bugColor").Int(3)
			);

			double delay = json.root.Float("delay");
			int mapSize = json.root.Int("mapSize");
			int numBugs = json.root.Int("numBugs");
			int numWalls = json.root.Int("numWalls");

			_world.SetDelayBetweenUpdates(delay);
			_world.GetMap().Resize(mapSize, mapSize);
			for (int i = 0; i < numBugs; ++i)
			{
				_world.SpawnBug(NSL::Vector2i(NSL::Random(0, mapSize - 1),
					NSL::Random(0, mapSize - 1)));
			}

			for (int i = 0; i < numWalls; ++i)
			{
				_world.GetMap()
					[NSL::Random(0, mapSize - 1)]
					[NSL::Random(0, mapSize - 1)].type = TileGrid::Tile::Type::Wall;
			}

			// ------------------------- Инициалзация базового контекста ------------------------ //
			renderUI = true;
			renderWindowState = true;
			//UI.SetScale(monitor.GetPPM() / 4);
			UI.LoadFont("C:/Windows/Fonts/CascadiaMono.ttf", monitor.GetPPM() * 8.0f);
			Renderer::Camera& camera = _scene.camera;
			camera.SetWidthAndHeight(GetFramebufferWidth(), GetFramebufferHeight());
			camera.SetFOVClampingValue(NSL::Vector2(1.0f, 100000.0f));
			camera.SetFOV(28.65f);
			//camera.SetFOV(100000.0f);
			camera.SetProjectionType(Renderer::Camera::ProjectionType::Orthographic);
			camera.SetDepthNear(-1000.0f);
			renderHandler.SetScale(1.0f);
			renderHandler.SetClearColor(NSL::Vector4(0.5f, 0.9f, 1.0f, 1.0f));
			renderHandler.postProcessParameters.kernelMatrix = NSL::Matrix3x3(-1.0f, -1.0f, -1.0f,
																			  -1.0f,  9.0f, -1.0f,
																			  -1.0f, -1.0f, -1.0f);

			// -------------------------------- Загрузка ресурсов ------------------------------- //
			Renderer::Texture2D texture;
			texture.width = mapSize;
			texture.height = mapSize;
			texture.channels = Renderer::Texture2D::Channels::RGB;
			texture.colorSpace = Renderer::Texture2D::ColorSpace::Linear;
			texture.UpdatePixelsData();
			renderHandler.AddTexture2D("Map", texture, Renderer::Texture2DFiltering::Nearest);

			renderHandler.CreateSprite("Map", "Map");
			_scene.sprites.push_back("Map");

			// -------------------------- Инициализация игровой логики -------------------------- //
			_FullyRedrawTextureFromMap();
		}
		virtual void Update(double delta) NSL_NOEXCEPT override
		{
			static const float speed = 0.01f;
			static const float fovSpeed = 0.1f;

			_world.Update(delta);
			_UpdateTextureFromMap();

			if (IsKeyboardKeyDown(IO::KeyboardKey::W))
			{
				_scene.camera.AddToPosition(NSL::Vector3::UnitY * speed);
			}
			if (IsKeyboardKeyDown(IO::KeyboardKey::S))
			{
				_scene.camera.AddToPosition(-NSL::Vector3::UnitY * speed);
			}
			if (IsKeyboardKeyDown(IO::KeyboardKey::A))
			{
				_scene.camera.AddToPosition(-NSL::Vector3::UnitX * speed);
			}
			if (IsKeyboardKeyDown(IO::KeyboardKey::D))
			{
				_scene.camera.AddToPosition(NSL::Vector3::UnitX * speed);
			}
			if (IsKeyboardKeyDown(IO::KeyboardKey::Up))
			{
				_scene.camera.AddToFOV(fovSpeed);
			}
			if (IsKeyboardKeyDown(IO::KeyboardKey::Down))
			{
				_scene.camera.AddToFOV(-fovSpeed);
			}
			if (IsMouseButtonDown(IO::MouseButton::Middle))
			{
				_scene.camera.InputMouseDelta(GetCursorPositionDelta().x, GetCursorPositionDelta().y);
			}
		}
		virtual void Render(double delta) NSL_NOEXCEPT override
		{
			renderHandler.Clear();

			//auto& tileTexture = renderHandler.GetTexture2D("Map");
			//UI.RenderViewportWindow(tileTexture.GetID());

			renderHandler.Render(_scene);
			//renderHandler.RenderInstancedMap(_scene, instances);
			//_RenderGameStatistics();
		}
		virtual void Unload() NSL_NOEXCEPT override
		{
			renderHandler.DeleteTexture2D("Map");
		}

	protected:
		virtual void FramebufferSizeCallback(int width, int height) NSL_NOEXCEPT override
		{
			GameWindow::FramebufferSizeCallback(width, height);
			_scene.camera.SetWidthAndHeight(width, height);
			renderHandler.SetViewport(width, height);
		}

	private:
		void _FullyRedrawTextureFromMap() NSL_NOEXCEPT
		{
			auto& tileTexture = renderHandler.GetTexture2D("Map");
			const auto& map = _world.GetMap();

			for (int x = 0; x < map.GetWidth(); ++x)
			{
				for (int y = 0; y < map.GetHeight(); ++y)
				{
					const TileGrid::Tile& tile = map[x][y];

					switch (tile.type)
					{
					case TileGrid::Tile::Type::Empty:
						tileTexture.SetPixelColor(x, y, _emptyColor);
						break;

					case TileGrid::Tile::Type::Wall:
						tileTexture.SetPixelColor(x, y, _wallColor);
						break;

					default:
						LogError("Unknown Tile Type");
						break;
					}
				}
			}

			for (const auto& bug : _world.GetBugs())
			{
				tileTexture.SetPixelColor(bug.GetX(), bug.GetY(), _bugColor);
			}
			renderHandler.UpdateTexture2D("Map");
		}
		void _UpdateTextureFromMap()
		{
			auto& tileTexture = renderHandler.GetTexture2D("Map");

			auto& bugs = _world.GetBugs();
			auto& bugsPreviousState = _world.GetBugsPreviousState();
			auto& movedBugsIndices = _world.GetMovedBugsIndices();

			for (auto& i : movedBugsIndices)
			{
				auto& bugPreviousState = bugsPreviousState[i];
				auto& bug = bugs[i];

				tileTexture.SetPixelColor(bugPreviousState.GetX(), bugPreviousState.GetY(), _emptyColor);
				tileTexture.SetPixelColor(bug.GetX(), bug.GetY(), _bugColor);
			}
			
			renderHandler.UpdateTexture2D("Map");
		}
		void _RenderGameStatistics() NSL_NOEXCEPT
		{
			UI.Begin("Game Statistics");

			UI.Text("Map width : ");
			UI.SameLine();
			UI.Int(_world.GetMap().GetWidth());
			UI.Text("Map height : ");
			UI.SameLine();
			UI.Int(_world.GetMap().GetHeight());

			UI.Text("Num of bugs : ");
			UI.SameLine();
			UI.Int(static_cast<int>(_world.GetBugs().size()));
			UI.End();
		}

	private:
		World _world;
		Renderer::Scene _scene;

		NSL::Color4 _emptyColor;
		NSL::Color4 _wallColor;
		NSL::Color4 _bugColor;
	};
} 