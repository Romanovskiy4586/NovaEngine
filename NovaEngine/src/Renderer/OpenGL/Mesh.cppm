export module Mesh;
#include "Core.h"
import std;
import NSL;

export namespace Renderer
{
	struct NSL_API Mesh final
	{
		Mesh(std::vector<float> vertices = std::vector<float>(), char coordinatesPerVertex = 3, std::vector<float> uvs = std::vector<float>(),
			std::vector<float> normals = std::vector<float>(), std::vector<unsigned int> indices = std::vector<unsigned int>(),
			std::vector<unsigned int> attributePointers = std::vector<unsigned int>()) NSL_NOEXCEPT
			: vertices(vertices)
			, coordinatesPerVertex(coordinatesPerVertex)
			, uvs(uvs)
			, normals(normals)
			, indices(indices)
			, attributePointers(attributePointers)
			, _vaoID(0)
			, _vboID(0)
			, _eboID(0)
			, _eboSize(0)
		{
		}
		~Mesh() NSL_NOEXCEPT
		{
			if (_vaoID) LogWarning("Mesh object is deleted, but it's VAO ID is not 0. VAO ID = " + std::to_string(_vaoID));
		}

		unsigned int GetVAOID() const NSL_NOEXCEPT
		{
			return _vaoID;
		}
		unsigned int GetEBOSize() const NSL_NOEXCEPT
		{
			return _eboSize;
		}

		std::vector<float> vertices;
		std::vector<float> uvs;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> attributePointers;
		char coordinatesPerVertex;

	private:
		unsigned int _vaoID;
		unsigned int _vboID;
		unsigned int _eboID;
		unsigned int _eboSize;

		friend class GLBuffersContextManager;
	};
}