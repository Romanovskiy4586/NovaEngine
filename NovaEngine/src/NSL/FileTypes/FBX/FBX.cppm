export module FBX;
#include "Core.h"
import std;

export namespace NSL
{
	struct NSL_API FBX
	{
		std::vector<double> vertices;
		std::vector<unsigned int> indices;
		std::vector<double> uvs;
		std::vector<double> normals;
	};
}