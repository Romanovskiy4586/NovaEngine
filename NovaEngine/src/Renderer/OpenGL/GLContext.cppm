export module GLContext;
#include "Core.h"

export namespace Renderer
{
	struct NSL_API GLContext final
	{
		GLContext() NSL_NOEXCEPT
			: cullFaceEnabled(true)
			, depthTestEnabled(true)
			, blendEnabled(true)
			, polygonFill(true)
			, stencilTest(true)
		{
		}

		bool cullFaceEnabled;
		bool depthTestEnabled;
		bool blendEnabled;
		bool polygonFill;
		bool stencilTest;
	};
}