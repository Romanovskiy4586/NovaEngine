export module OpenGLIndirectCommands;
#include "Core.h"

export namespace Renderer
{
	typedef struct
	{
		unsigned int count;
		unsigned int primCount;
		unsigned int first;
		unsigned int baseInstance;
	}
	DrawArraysIndirectCommand;

	typedef struct
	{
		unsigned int count;
		unsigned int primCount;
		unsigned int firstIndex;
		int baseVertex;
		unsigned int baseInstance;
	}
	DrawElementsIndirectCommand;
}