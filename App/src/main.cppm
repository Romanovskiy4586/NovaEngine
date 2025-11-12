module;
#include "Core.h"
#ifdef OS_WINDOWS
#include <Windows.h>
#endif
export module main;

import std;
import NovaEngine;
import StalkerGame;

export NSL_MAIN
{
	Engine::Init();

	{
		Stalker::StalkerGame game;
		game.Run();
	}

	return Engine::Exit();
}