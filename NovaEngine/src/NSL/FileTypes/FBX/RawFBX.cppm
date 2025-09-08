export module RawFBX;
#include "Core.h"
import std;

export namespace NSL
{
	struct NSL_API RawFBX
	{
		enum class NSL_API MappingType { ByPolygon, ByPolygonVertex, ByVertex, ByEdge, AllSame };
		enum class NSL_API ReferenceType { Direct, IndexToDirect };

		std::vector<double> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<double> UVs;
		std::vector<unsigned int> UVsIndices;
		std::vector<double> Normals;
		std::vector<unsigned int> NormalsIndices;
		MappingType UVsMapping;
		ReferenceType UVsReference;
		MappingType NormalsMapping;
		ReferenceType NormalsReference;
	};
}