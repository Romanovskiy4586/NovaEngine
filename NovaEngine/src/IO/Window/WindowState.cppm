export module WindowState;
#include "Core.h"
import std;
import NSL;

static const std::string s_jsonPath = "WindowState.json";

export namespace IO
{
	struct NSL_API WindowState
	{
		WindowState() NSL_NOEXCEPT
			: name()
			, width(1600)
			, height(900)
			, positionX(0)
			, positionY(40)
			, maximized(false)
			, resizable(true)
			, fullscreen(false)
			, vsync(true)
			, fpsLock(0)
			, cursorGrabbed(false)
			, framebufferWidth(0)
			, framebufferHeight(0)
		{
			if (!NSL::IsValidFilePath("WindowState.json")) return;

			NSL::JSON json = NSL::JSON::Load("WindowState.json");
			name = json.root.String("name");
			width = static_cast<int>(json.root.Int("width"));
			height = static_cast<int>(json.root.Int("height"));
			positionX = static_cast<int>(json.root.Int("positionX"));
			positionY = static_cast<int>(json.root.Int("positionY"));
			resizable = json.root.Bool("resizable");
			fullscreen = json.root.Bool("fullscreen");
			vsync = json.root.Bool("vsync");
			fpsLock = static_cast<int>(json.root.Int("fpsLock"));
			cursorGrabbed = json.root.Bool("cursorGrabbed");
			maximized = json.root.Bool("maximized");
		}
		~WindowState() NSL_NOEXCEPT
		{
			AsJSON().Save("WindowState.json");
		}

		NSL::JSON AsJSON() const NSL_NOEXCEPT
		{
			NSL::JSON json;

			json.root.AddProperty("name", name);
			json.root.AddProperty("width", width);
			json.root.AddProperty("height", height);
			json.root.AddProperty("positionX", positionX);
			json.root.AddProperty("positionY", positionY);
			json.root.AddProperty("resizable", resizable);
			json.root.AddProperty("fullscreen", fullscreen);
			json.root.AddProperty("vsync", vsync);
			json.root.AddProperty("fpsLock", fpsLock);
			json.root.AddProperty("cursorGrabbed", cursorGrabbed);
			json.root.AddProperty("maximized", maximized);

			return json;
		}

		std::string name;
		int width;
		int height;
		int positionX;
		int positionY;
		bool maximized;
		bool resizable;
		bool fullscreen;
		bool vsync;
		int fpsLock;
		bool cursorGrabbed;

		// Properties below do not save in .json
		int framebufferWidth;
		int framebufferHeight;
	};
}