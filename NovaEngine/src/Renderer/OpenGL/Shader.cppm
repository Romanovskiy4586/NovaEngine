export module Shader;
#include "Core.h"
import std;
import NSL;

export namespace Renderer
{
    struct NSL_API Shader final
    {
        Shader(const std::string& vertesSourceCode = std::string(), const std::string& geometrySourceCode = std::string(),
            const std::string& fragmentSourceCode = std::string()) NSL_NOEXCEPT
            : _id(0)
            , _uniforms()
        {
            
        }
        ~Shader() NSL_NOEXCEPT
        {
            if (_id) LogWarning("Shader object is deleted, but it's ID is not 0. ID = " + std::to_string(_id));
        }

        unsigned int GetID() const NSL_NOEXCEPT
        {
            return _id;
        }
        int GetUniformID(const std::string& name) const NSL_NOEXCEPT
        {
            return _uniforms.at(name);
        }
        bool UniformIsPresent(const std::string& name) const NSL_NOEXCEPT
        {
            return _uniforms.contains(name);
        }

        std::string vertexSourceCode;
        std::string geometrySourceCode;
        std::string fragmentSourceCode;

    private:
        unsigned int _id;
        std::unordered_map<std::string, int> _uniforms;

        friend class GLShadersManager;
    };
}