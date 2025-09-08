module;
#include "glfw3.h"
export module MouseButtons;
#include "Core.h"

import NSL;

export namespace IO
{
	enum class NSL_API MouseButton
	{
		Left =		GLFW_MOUSE_BUTTON_LEFT,
		Right =		GLFW_MOUSE_BUTTON_RIGHT,
		Middle =	GLFW_MOUSE_BUTTON_MIDDLE
	};
}