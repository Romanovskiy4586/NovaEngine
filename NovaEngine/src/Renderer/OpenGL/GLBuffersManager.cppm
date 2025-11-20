export module GLBuffersManager;
#include "Core.h"
#include "glad.h"
import std;
import NSL;
import Mesh;
import DrawCommandBuffer;
import OpenGLIndirectCommands;
import ShaderStorageBuffer;

export namespace Renderer
{
	class NSL_API GLBuffersManager
	{
	public:
		GLBuffersManager()
			: _bindedVAO(0)
			, _bindedUBO(0)
		{
		}

	public:
		void RegisterMesh(Mesh& mesh) NSL_NOEXCEPT
		{
			// Init members
			mesh._eboSize = static_cast<unsigned int>(mesh.indices.size());

			// Prepare to create VBO data
			size_t totalDataSize = mesh.vertices.size() + mesh.uvs.size() + mesh.normals.size();
			std::vector<float> vboData(totalDataSize);
			auto verticiesPointer = mesh.vertices.begin();
			auto uvsPointer = mesh.uvs.begin();
			auto normalsPointer = mesh.normals.begin();
			auto vboPointer = vboData.begin();

			// Create VBO data
			while (vboPointer != vboData.end())
			{
				// Vertices
				for (char i = 0; i < mesh.coordinatesPerVertex; ++i)
				{
					*vboPointer++ = *verticiesPointer++;
				}

				// UVs
				if (!mesh.uvs.empty())
				{
					for (char i = 0; i < 2; ++i)
					{
						*vboPointer++ = *uvsPointer++;
					}
				}

				// Normals
				if (!mesh.normals.empty())
				{
					for (char i = 0; i < 3; ++i)
					{
						*vboPointer++ = *normalsPointer++;
					}
				}
			}

			// Generate VAO, VBO and EBO
			GLCall(glGenVertexArrays(1, &mesh._vaoID));
			GLCall(glGenBuffers(1, &mesh._vboID));
			GLCall(glGenBuffers(1, &mesh._eboID));

			GLCall(glBindVertexArray(mesh._vaoID));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, mesh._vboID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, vboData.size() * sizeof(GLfloat), vboData.data(), GL_STATIC_DRAW));

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._eboID));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), mesh.indices.data(), GL_STATIC_DRAW));

			// Set Attribute Pointers
			int arrSum = 0;
			for (auto& i : mesh.attributePointers)
			{
				arrSum += i;
			}

			int buffer = 0;
			for (int i = 0; i < mesh.attributePointers.size(); ++i)
			{
				GLint currentAttributePointer = mesh.attributePointers[i];
				GLCall(glEnableVertexAttribArray(i));
				GLCall(glVertexAttribPointer(i, currentAttributePointer, GL_FLOAT, GL_FALSE, arrSum * sizeof(GLfloat), reinterpret_cast<GLvoid*>(buffer * sizeof(GLfloat))));
				buffer += currentAttributePointer;
			}
		}
		void BindMesh(const Mesh* mesh) NSL_NOEXCEPT
		{
			if (_bindedVAO == mesh->GetVAOID()) return;

			_bindedVAO = mesh->GetVAOID();
			GLCall(glBindVertexArray(_bindedVAO));
		}
		void FreeMesh(Mesh& mesh) NSL_NOEXCEPT
		{
			if (_bindedVAO == mesh.GetVAOID()) _bindedVAO = 0;

			// Delete VAO
			if (mesh._vaoID)
			{
				//GLCall(glBindVertexArray(0));
				GLCall(glDeleteVertexArrays(1, &mesh._vaoID));
				mesh._vaoID = 0;
			}

			// Delete VBO
			if (mesh._vboID)
			{
				//GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				GLCall(glDeleteBuffers(1, &mesh._vboID));
				mesh._vboID = 0;
			}

			// Delete EBO
			if (mesh._eboID)
			{
				//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
				GLCall(glDeleteBuffers(1, &mesh._eboID));
				mesh._eboID = 0;
			}
		}

		//void RegisterUniformBuffer(UniformBuffer& uniformBuffer) NSL_NOEXCEPT
		//{
		//	GLCall(glGenBuffers(1, &uniformBuffer._id));
		//	BindUniformBuffer(&uniformBuffer);
		//	GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(decltype(uniformBuffer.data)::value_type) * uniformBuffer.data.size(),
		//		uniformBuffer.data.data(), GL_DYNAMIC_READ));
		//}
		//void BindUniformBuffer(const UniformBuffer* uniformBuffer) NSL_NOEXCEPT
		//{
		//	if (_bindedUBO == uniformBuffer->_id) return;
		//
		//	_bindedUBO = uniformBuffer->_id;
		//	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, _bindedUBO));
		//}

		void RegisterShaderStorageBuffer(ShaderStorageBuffer& shaderStorageBuffer) NSL_NOEXCEPT
		{
			GLCall(glGenBuffers(1, &shaderStorageBuffer._id));
			_BindShaderStorageBuffer(&shaderStorageBuffer, 0);
			GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER,
				shaderStorageBuffer.Size(),
				shaderStorageBuffer.Data(), GL_DYNAMIC_READ));
		}
		void BindShaderStorageBuffer(const ShaderStorageBuffer* shaderStorageBuffer, unsigned int index) NSL_NOEXCEPT
		{
			NSL_ASSERT(index <= _bindedSSBOs.size() - 1,
				"Index for binding ShaderStorageBuffer is bigger than max " + std::to_string(_bindedSSBOs.size() - 1) +
				". Index: " + std::to_string(index))
			_BindShaderStorageBuffer(shaderStorageBuffer, index);
		}
		void UpdateShaderStorageBuffer(ShaderStorageBuffer& shaderStorageBuffer) NSL_NOEXCEPT
		{
			_BindShaderStorageBuffer(&shaderStorageBuffer, 0);
			GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
				shaderStorageBuffer.Size(),
				shaderStorageBuffer.Data()));
		}
		void ReloadShaderStorageBuffer(ShaderStorageBuffer& shaderStorageBuffer) NSL_NOEXCEPT
		{
			_BindShaderStorageBuffer(&shaderStorageBuffer, 0);
			GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER,
				shaderStorageBuffer.Size(),
				shaderStorageBuffer.Data(), GL_DYNAMIC_READ));
		}
		void FreeShaderStorageBuffer(ShaderStorageBuffer& shaderStorageBuffer) NSL_NOEXCEPT
		{
			GLCall(glDeleteBuffers(1, &shaderStorageBuffer._id));
			shaderStorageBuffer._id = 0;
		}

		//void UpdateMesh(Mesh& mesh) NSL_NOEXCEPT
		//{
		//	glBindBuffer(GL_ARRAY_BUFFER, mesh._vboID);
		//	void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, gpuEntities.size() * sizeof(GPUEntity),
		//		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		//	memcpy(ptr, gpuEntities.data(), gpuEntities.size() * sizeof(GPUEntity));
		//	glUnmapBuffer(GL_ARRAY_BUFFER);
		//}

		void DrawCall(const Mesh& mesh) NSL_NOEXCEPT
		{
			BindMesh(&mesh);
			GLCall(glDrawElements(GL_TRIANGLES, mesh.GetEBOSize(), GL_UNSIGNED_INT, 0));
		}
		void DrawCall(const Mesh& mesh, unsigned int instanceCount) NSL_NOEXCEPT
		{
			BindMesh(&mesh);
			GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh.GetEBOSize(), GL_UNSIGNED_INT, 0, instanceCount));
		}
		void DrawIndirect(const Mesh& mesh, const DrawCommandBuffer& buffer) NSL_NOEXCEPT
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._eboID);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer._id);

			// This will submit all commands.size() draw commands in the currently
			// bound buffer
			glMultiDrawElementsIndirect(
				GL_TRIANGLES,
				GL_UNSIGNED_INT, // Type of data in indicesBuffer
				(const void*)0, // No offset into draw command buffer
				static_cast<GLsizei>(buffer.GetCommands().size()),
				0                // No stride as data is tightly packed
			);
		}

	private:
		void _BindShaderStorageBuffer(const ShaderStorageBuffer* shaderStorageBuffer, unsigned int index) NSL_NOEXCEPT
		{
			if (_bindedSSBOs[index] == shaderStorageBuffer->_id) return;

			_bindedSSBOs[index] = shaderStorageBuffer->_id;
			GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, shaderStorageBuffer->_id));
		}

	private:
		unsigned int _bindedVAO;
		unsigned int _bindedUBO;
		std::array<unsigned int, 32> _bindedSSBOs;
	};
}