export module GLContextManager;
#include "Core.h"
#include "glad.h"
import std;
import NSL;
import GLContext;
import Shader;
//import Framebuffer;
import Texture2D;
import Pixelbuffer;

export namespace Renderer
{
	// При создании инициализирует базовый глобальный контекст: вьюпорт, маску stencil, blending, clear color, включается multisample.
	// С помощью объекта этого класса меняется глобальный контекст, в лупе вызывается отрисовка мешей
	class NSL_API GLContextManager final
	{
	public:
		GLContextManager(int framebufferWidth, int framebufferHeight) NSL_NOEXCEPT
			: _clearColor(0.0f)
			, _currentContext()
			, _viewportX(0)
			, _viewportY(0)
			, _viewportWidth(framebufferWidth)
			, _viewportHeight(framebufferHeight)

		{
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			GLCall(glStencilMask(0xFF));
			GLCall(glEnable(GL_MULTISAMPLE));
			GLCall(glViewport(0, 0, _viewportWidth, _viewportHeight));

			GLCall(glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w));
			_InitContext();

			glGenQueries(1, &_gpuTimerID);

			LogInfo("GLContextManager is created");
		}

	public:
		void SetViewport(int width, int height) NSL_NOEXCEPT
		{
			SetViewport(0, 0, width, height);
		}
		void SetViewport(int x, int y, int width, int height) NSL_NOEXCEPT
		{
			if (_viewportX == x && _viewportY == y && _viewportWidth == width && _viewportHeight == height) return;

			NSL_DEBUG
			(
				if (width < 0) LogError("Viewport width settet with negative value: " + std::to_string(width));
				if (height < 0) LogError("Viewport height settet with negative value: " + std::to_string(height));

				if (x < 0) LogWarning("Viewport x settet with negative value: " + std::to_string(width));
				if (y < 0) LogWarning("Viewport y settet with negative value: " + std::to_string(height));
			)

			_viewportX = x;
			_viewportY = y;
			_viewportWidth = width;
			_viewportHeight = height;

			GLCall(glViewport(x, y, width, height));
		}

		void SetClearColor(const NSL::Vector4& color) NSL_NOEXCEPT
		{
			if (_clearColor == color) return;

			_clearColor = color;
			GLCall(glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w));
		}
		void SetContext(const GLContext& context) NSL_NOEXCEPT
		{
			if (context.cullFaceEnabled != _currentContext.cullFaceEnabled)		_SetCullFace(context.cullFaceEnabled);
			if (context.depthTestEnabled != _currentContext.depthTestEnabled)	_SetDepthTest(context.depthTestEnabled);
			if (context.blendEnabled != _currentContext.blendEnabled)			_SetBlend(context.blendEnabled);
			if (context.polygonFill != _currentContext.polygonFill)				_SetPolygonFill(context.polygonFill);
			if (context.stencilTest != _currentContext.stencilTest)				_SetStencilTest(context.stencilTest);
		}
		void Clear() NSL_NOEXCEPT
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		}

		void BindPixelbuffer(Pixelbuffer* pixelbuffer) NSL_NOEXCEPT
		{
			if (_bindedPixelbuffer == pixelbuffer) return;

			_bindedPixelbuffer = pixelbuffer;
			GLCall(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelbuffer ? pixelbuffer->GetID() : 0));
		}

		void StartMeasuring() NSL_NOEXCEPT
		{
			glBeginQuery(GL_TIME_ELAPSED, _gpuTimerID);
		}
		float EndMeasuring() NSL_NOEXCEPT
		{
			static GLuint64 gpuTimeNs;

			glEndQuery(GL_TIME_ELAPSED);
			glGetQueryObjectui64v(_gpuTimerID, GL_QUERY_RESULT, &gpuTimeNs);
			return float(gpuTimeNs) / 1e6f;
		}

	private:
		void _InitContext() NSL_NOEXCEPT
		{
			_SetCullFace(_currentContext.cullFaceEnabled);
			_SetDepthTest(_currentContext.depthTestEnabled);
			_SetBlend(_currentContext.blendEnabled);
			_SetPolygonFill(_currentContext.polygonFill);
			_SetStencilTest(_currentContext.stencilTest);
		}
		
		void _SetCullFace(bool value) NSL_NOEXCEPT
		{
			_currentContext.cullFaceEnabled = value;
			if (value)
			{
				GLCall(glEnable(GL_CULL_FACE));
			}
			else
			{
				GLCall(glDisable(GL_CULL_FACE));
			}
		}
		void _SetDepthTest(bool value) NSL_NOEXCEPT
		{
			_currentContext.depthTestEnabled = value;
			if (value)
			{
				GLCall(glEnable(GL_DEPTH_TEST));
			}
			else
			{
				GLCall(glDisable(GL_DEPTH_TEST));
			}
		}
		void _SetBlend(bool value) NSL_NOEXCEPT
		{
			_currentContext.blendEnabled = value;
			if (value)
			{
				GLCall(glEnable(GL_BLEND));
			}
			else
			{
				GLCall(glDisable(GL_BLEND));
			}
		}
		void _SetPolygonFill(bool value) NSL_NOEXCEPT
		{
			_currentContext.polygonFill = value;
			if (value)
			{
				GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
			}
			else
			{
				GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
			}
		}
		void _SetStencilTest(bool value) NSL_NOEXCEPT
		{
			_currentContext.stencilTest = value;
			if (value)
			{
				GLCall(glEnable(GL_STENCIL_TEST));
			}
			else
			{
				GLCall(glDisable(GL_STENCIL_TEST));
			}
		}

	private:
		NSL::Vector4 _clearColor;
		GLContext _currentContext;
		GLuint _gpuTimerID;
		
		//Shader* _bindedShader;
		//Framebuffer* _bindedFramebuffer;
		Pixelbuffer* _bindedPixelbuffer;
		int _viewportX;
		int _viewportY;
		int _viewportWidth;
		int _viewportHeight;
	};
}