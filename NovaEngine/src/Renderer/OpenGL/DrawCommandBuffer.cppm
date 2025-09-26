export module DrawCommandBuffer;
#include "Core.h"
import std;
import NSL;
import OpenGLIndirectCommands;

export namespace Renderer
{
	class NSL_API DrawCommandBuffer
	{
	public:
		DrawCommandBuffer() NSL_NOEXCEPT
			: _id(0)
		{
			////code that fills commands vector with DrawElementsIndirectCommand elements ..
			//// (for example)
			//commands.push_back(mesh1Cmd);
			//commands.push_back(mesh2Cmd);
			//commands.push_back(mesh3Cmd);

			//glCreateBuffers(1, &_id);

			//glNamedBufferStorage(_id,
			//	sizeof(DrawElementsIndirectCommand) * commands.size(),
			//	(const void*)commands.data(),
			//	GL_DYNAMIC_STORAGE_BIT);
		}

		std::vector<DrawElementsIndirectCommand>& GetCommands() NSL_NOEXCEPT
		{
			return _commands;
		}

		const std::vector<DrawElementsIndirectCommand>& GetCommands() const NSL_NOEXCEPT
		{
			return _commands;
		}

	private:
		std::vector<DrawElementsIndirectCommand> _commands;
		unsigned int _id;
		friend class GLBuffersManager;
	};
}