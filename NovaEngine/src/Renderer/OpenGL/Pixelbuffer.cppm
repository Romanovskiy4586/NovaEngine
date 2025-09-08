export module Pixelbuffer;
#include "Core.h"
#include "glad.h"
import std;
import NSL;

export namespace Renderer
{
	class NSL_API Pixelbuffer
	{
	public:
		struct NSL_API Data
		{
			enum NSL_API Channels : char { RED = 1, RG = 2, RGB = 3, RGBA = 4 };

			Data(int width = 1, int height = 1, Channels channels = Channels::RED)
				: width(width)
				, height(height)
				, channels(channels)
			{
			}

			int width;
			int height;
			Channels channels;
		};

	public:
		Pixelbuffer()
			: Pixelbuffer(Data())
		{
		}
		Pixelbuffer(const Data& data) NSL_NOEXCEPT
		{
			NSL_ASSERT(data.width >= 1, "Pixelbuffer width cannot be less than 1. Width parameter: " + std::to_string(data.width))
			NSL_ASSERT(data.height >= 1, "Pixelbuffer height cannot be less than 1. Height parameter: " + std::to_string(data.height))
			NSL_ASSERT(data.channels >= 1, "Pixelbuffer channels cannot be less than 1. Channels parameter: " + std::to_string(data.channels))
			NSL_ASSERT(data.channels <= 4, "Pixelbuffer channels cannot be greater than 4. Channels parameter: " + std::to_string(data.channels))

			glGenBuffers(1, &_id);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, _id);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, data.width * data.height * data.channels, nullptr, GL_STREAM_DRAW);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		}
		~Pixelbuffer() NSL_NOEXCEPT
		{
			glDeleteBuffers(1, &_id);
		}

		GLuint GetID() const NSL_NOEXCEPT
		{
			return _id;
		}

	private:
		GLuint _id;
	};
}