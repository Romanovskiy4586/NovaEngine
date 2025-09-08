export module Texture2D;
#include "Core.h"
import std;
import NSL;

export namespace Renderer
{
	struct NSL_API Texture2D final
	{
		enum NSL_API Channels : int { RED = 1, RG = 2, RGB = 3, RGBA = 4 };
		enum class NSL_API ColorSpace : int { Linear, SRGB };

		Texture2D(int width = 1, int height = 1, Channels channels = Channels::RED, 
			ColorSpace colorspace = ColorSpace::Linear, const std::vector<unsigned char> pixels = std::vector<unsigned char>())
			: _id(0)
			, width(width)
			, height(height)
			, channels(channels)
			, colorSpace(colorspace)
			, pixels(pixels)
		{
			if (pixels.empty()) UpdatePixelsData();
		}
		~Texture2D() NSL_NOEXCEPT
		{
			if (_id) LogWarning("Texture object is deleted, but it's ID is not 0. ID = " + std::to_string(_id));
		}

		unsigned int GetID() const NSL_NOEXCEPT
		{
			return _id;
		}
		const void* GetPixels() const NSL_NOEXCEPT
		{
			return pixels.empty() ? nullptr : pixels.data();
		}
		void UpdatePixelsData() NSL_NOEXCEPT
		{
			pixels.resize(width * height * channels);
		}
		void SetPixelColor(int x, int y, const NSL::Color4& color) NSL_NOEXCEPT
		{
			NSL_ASSERT(x >= 0 && x < width, "X coordinate is out of bounds. Texture width: " + std::to_string(width) + ". Input \"x\" index: " + std::to_string(x))
			NSL_ASSERT(y >= 0 && y < height, "Y coordinate is out of bounds. Texture height: " + std::to_string(height) + ". Input \"y\" index: " + std::to_string(y))

			const unsigned char& r = color.r;
			const unsigned char& g = color.g;
			const unsigned char& b = color.b;
			const unsigned char& a = color.a;

			size_t pixelIndex = y * width + x;

			if (channels == Channels::RED)
			{
				unsigned char& currentR = pixels[pixelIndex * 1 + 0];
				if (currentR != r) currentR = r;
				return;
			}
			if (channels == Channels::RG)
			{
				unsigned char& currentR = pixels[pixelIndex * 2 + 0];
				unsigned char& currentG = pixels[pixelIndex * 2 + 1];
				if (currentR != r) currentR = r;
				if (currentG != g) currentG = g;
				return;
			}
			if (channels == Channels::RGB)
			{
				unsigned char& currentR = pixels[pixelIndex * 3 + 0];
				unsigned char& currentG = pixels[pixelIndex * 3 + 1];
				unsigned char& currentB = pixels[pixelIndex * 3 + 2];
				if (currentR != r) currentR = r;
				if (currentG != g) currentG = g;
				if (currentB != b) currentB = b;
				return;
			}
			if (channels == Channels::RGBA)
			{
				unsigned char& currentR = pixels[pixelIndex * 4 + 0];
				unsigned char& currentG = pixels[pixelIndex * 4 + 1];
				unsigned char& currentB = pixels[pixelIndex * 4 + 2];
				unsigned char& currentA = pixels[pixelIndex * 4 + 3];
				if (currentR != r) currentR = r;
				if (currentG != g) currentG = g;
				if (currentB != b) currentB = b;
				if (currentA != a) currentA = a;
				return;
			}
		}
		void Clear(const NSL::Color4 color = NSL::Color4::Black) NSL_NOEXCEPT
		{
			_Clear(color);
		}
		//void UpdatePixelOnGPU(int x, int y) NSL_NOEXCEPT
		//{
			//GLCall(glTextureSubImage2D(_id, 0, x, y, 1, 1, _glFormat, GL_UNSIGNED_BYTE, static_cast<const void*>(_pixels.data() + y * _width + x)));
		//}
		//void ReloadOnGPUWithPixelbuffer() NSL_NOEXCEPT
		//{
			//static Pixelbuffer pixelbuffer(Pixelbuffer::Data(_width, _height, static_cast<Pixelbuffer::Data::Channels>(_channels)));
			//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelbuffer.GetID());

			//void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
			//memcpy(ptr, _pixels.data(), _width * _height * _channels);
			//glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

			//glBindTexture(GL_TEXTURE_2D, _id);
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _glFormat, GL_UNSIGNED_BYTE, 0);

			//glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		//}
		//void ReloadOnGPU() NSL_NOEXCEPT
		//{
			//GLCall(glBindTexture(GL_TEXTURE_2D, _id));
			//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, _glFormat, _width, _height, 0, _glFormat, GL_UNSIGNED_BYTE, static_cast<const void*>(_pixels.data())));
		//}

		int width;
		int height;
		Channels channels;
		ColorSpace colorSpace;
		std::vector<unsigned char> pixels;

	private:
		inline void _Clear(const NSL::Color4 color) NSL_NOEXCEPT
		{
			if (color.r == color.g && color.g == color.b && color.b == color.a)
			{
				pixels.resize(width * height * channels, color.r);
			}
			else
			{
				auto it = pixels.begin();
				auto end = pixels.end();
				int i;

				while (it != end)
				{
					for (i = 0; i < channels; ++i)
					{
						unsigned char c = color[i];
						if (*it == c)
						{
							++it;
							continue;
						}
						*it++ = c;
					}
				}
			}
		}

	private:
		unsigned int _id;
		friend class GLTexturesContextManager;
	};
}