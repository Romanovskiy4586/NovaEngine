export module Sprite;
#include "Core.h"
import std;
import Transform;
import GLContext;
import Texture2D;


export namespace Renderer
{
	struct NSL_API Sprite final
	{
		Sprite(Texture2D& texture) NSL_NOEXCEPT
			: texture(texture)
			, transform()
			, context()
		{
		}

		Texture2D& texture;
		Transform transform;
		GLContext context;
	};
}