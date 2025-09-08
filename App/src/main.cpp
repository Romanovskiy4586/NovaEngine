#include "Core.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#endif

import std;
import NovaEngine;

import StalkerWindow;

NSL_MAIN
{
	Engine::Init();

	{
		Stalker::StalkerWindow window;
		window.Run();
	}

	return Engine::Exit();
}