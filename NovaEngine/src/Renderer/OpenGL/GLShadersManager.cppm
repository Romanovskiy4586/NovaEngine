export module GLShadersManager;
#include "Core.h"
#include "glad.h"
import std;
import NSL;
import Shader;

export namespace Renderer
{
	class NSL_API GLShadersManager
	{
	public:
        GLShadersManager()
            : _bindedShader(0)
            , _bindedShaderUniforms()
		{
		}

	public:
		void BindShader(Shader* shader) NSL_NOEXCEPT
		{
            NSL_ASSERT(shader->_id, "Cannot bind shader, because it's ID is 0")
			if (_bindedShader == shader->_id) return;

            if (shader)
            {
                _bindedShader = shader->_id;
                _bindedShaderUniforms = shader->_uniforms;
            }
            else
            {
                _bindedShader = 0;
                _bindedShaderUniforms.clear();
            }

			GLCall(glUseProgram(_bindedShader));
		}
		void RegisterShader(Shader& shader) NSL_NOEXCEPT
		{
            NSL_ASSERT(!shader.vertexSourceCode.empty() && !shader.fragmentSourceCode.empty(),
                "Shader can not be initialized. Some important data arguments is empty")

            GLuint vertexShaderID = 0U;
            GLuint geometryShaderID = 0U;
            GLuint fragmentShaderID = 0U;

            if (!shader.vertexSourceCode.empty())
            {
                vertexShaderID = _CreateShader(shader.vertexSourceCode, GL_VERTEX_SHADER);
            }
            if (!shader.geometrySourceCode.empty())
            {
                geometryShaderID = _CreateShader(shader.geometrySourceCode, GL_GEOMETRY_SHADER);
            }
            if (!shader.fragmentSourceCode.empty())
            {
                fragmentShaderID = _CreateShader(shader.fragmentSourceCode, GL_FRAGMENT_SHADER);
            }

            shader._id = _CreateShaderProgram(vertexShaderID, geometryShaderID, fragmentShaderID);
            _MapUniforms(shader);
		}
        void FreeShader(Shader& shader)
        {
            if (_bindedShader == shader._id) 
            {
                _bindedShader = 0;
                _bindedShaderUniforms.clear();
            }
            GLCall(glDeleteProgram(shader._id));
            shader._id = 0;
        }

        void SetUniform(const NSL::Vector2& data, const std::string& uniformName) NSL_NOEXCEPT
        {
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
            GLCall(glUniform2f(_bindedShaderUniforms[uniformName], data.x, data.y));
        }
        void SetUniform(const std::vector<NSL::Vector2> data, const std::string& uniformName) NSL_NOEXCEPT
        {
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
            GLCall(glUniform2fv(_bindedShaderUniforms[uniformName], static_cast<GLsizei>(data.size()), (const float*)data.data()));
        }
		void SetUniform(const NSL::Vector3& data, const std::string& uniformName) NSL_NOEXCEPT
		{
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniform3f(_bindedShaderUniforms[uniformName], data.x, data.y, data.z));
		}
		void SetUniform(const NSL::Vector4& data, const std::string& uniformName) NSL_NOEXCEPT
		{
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniform4f(_bindedShaderUniforms[uniformName], data.x, data.y, data.z, data.w));
		}
		void SetUniform(int data, const std::string& uniformName) NSL_NOEXCEPT
		{
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniform1i(_bindedShaderUniforms[uniformName], data));
		}
		void SetUniform(bool data, const std::string& uniformName) NSL_NOEXCEPT
		{
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniform1i(_bindedShaderUniforms[uniformName], data));
		}
		void SetUniform(float data, const std::string& uniformName) NSL_NOEXCEPT
		{
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniform1f(_bindedShaderUniforms[uniformName], data));
		}
		void SetUniform(const NSL::Matrix4x4& data, const std::string& uniformName, bool transpose) NSL_NOEXCEPT
		{
			NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniformMatrix4fv(_bindedShaderUniforms[uniformName], 1, transpose, NSL::GetRawData(data).data()));
		}
		void SetUniform(const NSL::Matrix3x3& data, const std::string& uniformName, bool transpose) NSL_NOEXCEPT
		{
            NSL_ASSERT(_bindedShader, "Attempt to set uniform, but no shader is binded")
            NSL_ASSERT(_bindedShaderUniforms.contains(uniformName), "Binded shader has not uniform with name: " + uniformName)
			GLCall(glUniformMatrix3fv(_bindedShaderUniforms[uniformName], 1, transpose, NSL::GetRawData(data).data()));
		}

	private:
        GLuint _CreateShader(const std::string& shaderText, const GLenum& shaderType) NSL_NOEXCEPT
        {
            // Compiling Shader
            GLCall(GLuint result = glCreateShader(shaderType));
            const GLchar* str = shaderText.c_str();
            GLCall(glShaderSource(result, 1, &str, nullptr));
            GLCall(glCompileShader(result));

            NSL_DEBUG
            (
                // Checking on errors
                GLint success;
            GLCall(glGetShaderiv(result, GL_COMPILE_STATUS, &success));
            if (!success)
            {
                GLchar infoLog[512];
                GLCall(glGetShaderInfoLog(result, 512, 0, infoLog));
                LogError("Error occurred whilst compiling Shader. OpenGL Infolog: " + std::string(infoLog));
            }
                )

                return result;
        }
        GLuint _CreateShaderProgram(const GLint& vertexShaderID, const GLint& geometryShaderID, const GLint& fragmentShaderID) NSL_NOEXCEPT
        {
            // Linking shader program
            GLCall(GLuint result = glCreateProgram());
            if (vertexShaderID)
            {
                GLCall(glAttachShader(result, vertexShaderID));
            }
            if (geometryShaderID)
            {
                GLCall(glAttachShader(result, geometryShaderID));
            }
            if (fragmentShaderID)
            {
                GLCall(glAttachShader(result, fragmentShaderID));
            }
            GLCall(glLinkProgram(result));

            // Flushing shaders
            if (vertexShaderID)
            {
                GLCall(glDetachShader(result, vertexShaderID));
                GLCall(glDeleteShader(vertexShaderID));
            }
            if (geometryShaderID)
            {
                GLCall(glDetachShader(result, geometryShaderID));
                GLCall(glDeleteShader(geometryShaderID));
            }
            if (fragmentShaderID)
            {
                GLCall(glDetachShader(result, fragmentShaderID));
                GLCall(glDeleteShader(fragmentShaderID));
            }


            NSL_DEBUG
            (
                // Checking on errors
                GLint success;
            GLCall(glGetProgramiv(result, GL_LINK_STATUS, &success));
            if (!success)
            {
                GLchar infoLog[512];
                GLCall(glGetProgramInfoLog(result, 512, 0, infoLog));
                LogError("Error occurred whilst compiling Shader. OpenGL Infolog:\n" + std::string(infoLog));
            }
                )

                return result;
        }
        void _MapUniforms(Shader& shader) NSL_NOEXCEPT
        {
            // Init variables for work with uniforms
            GLint numberOfUniforms;
            GLint variableSize;
            GLenum variableType;
            const GLsizei nameMaxLength = 16;
            GLchar variableName[nameMaxLength];
            GLsizei nameLength;

            // Getting uniforms info and filling hashmap with it
            GLCall(glGetProgramiv(shader._id, GL_ACTIVE_UNIFORMS, &numberOfUniforms));
            for (GLint i = 0; i < numberOfUniforms; ++i)
            {
                GLCall(glGetActiveUniform(shader._id, (GLuint)i, nameMaxLength, &nameLength, &variableSize, &variableType, variableName));
                GLint location = glGetUniformLocation(shader._id, variableName);
                shader._uniforms[variableName] = location;
            }
        }

	private:
		unsigned int _bindedShader;
        std::unordered_map<std::string, int> _bindedShaderUniforms;
	};
}