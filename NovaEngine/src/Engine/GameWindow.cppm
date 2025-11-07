export module GameWindow;
#include "Core.h"
import std;
import NSL;
export import IO;
export import Renderer;
export import WorkThread;

export namespace Engine
{
	class NSL_API GameWindow final
	{
	public:
		GameWindow() NSL_NOEXCEPT
			: baseWindow()
			, renderHandler(baseWindow.GetFramebufferWidth(), baseWindow.GetFramebufferHeight())
		{
			baseWindow.SubscribeOnFramebuffer([this](int width, int height) { _OnFramebuffer(width, height); });
		}

	public:
		void Run() NSL_NOEXCEPT
		{
			load();

			while (!baseWindow.windowShouldClose)
			{
				baseWindow.StartFrame();
				update(baseWindow.GetDelta());
				render(baseWindow.GetDelta());
				baseWindow.EndFrame();
			}

			unload();
		}

	public:
		IO::BaseWindow baseWindow;
		Renderer::RenderHandler renderHandler;

		std::function<void()> load;
		std::function<void(double)> update;
		std::function<void(double)> render;
		std::function<void()> unload;

	private:
		void _OnFramebuffer(int width, int height) NSL_NOEXCEPT
		{
			renderHandler.UpdateFramebufferSize(width, height);
		}
	};
}