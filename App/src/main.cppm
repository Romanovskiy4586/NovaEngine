module;
#include "Core.h"
#ifdef OS_WINDOWS
#include <Windows.h>
#endif
export module main;

import std;
import NovaEngine;
import StalkerWindow;

export NSL_MAIN
{
	Engine::Init();

	{
		Stalker::StalkerWindow window;
		window.Run();
	}

	return Engine::Exit();
}