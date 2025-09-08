export module PNG;
#include "Core.h"
import std;

export namespace NSL
{
	struct NSL_API PNG
	{
		enum NSL_API Channels { RED = 1, RG = 2, RGB = 3, RGBA = 4 };
		enum class NSL_API ColorSpace { Linear, SRGB };

		//std::string name;
		std::vector<unsigned char> data;
		unsigned int width;
		unsigned int height;
		Channels channels;
		ColorSpace colorSpace;
		float gamma;
	};
}