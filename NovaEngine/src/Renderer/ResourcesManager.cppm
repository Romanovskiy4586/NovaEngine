export module ResourcesManager;
#include "Core.h"
#include "glad.h"
import std;
import NSL;
import Mesh;
import Texture2D;
import Shader;
//import Framebuffer;
import Pixelbuffer;
import ShaderStorageBuffer;

export namespace Renderer
{
	class NSL_API ResourcesManager final
		: public NSL::INonCopyable
		, public NSL::INonMovable
	{
	public:
		void LoadMesh(const std::string& name, const std::string& fbxPath) NSL_NOEXCEPT
		{
			NSL_ASSERT(!_meshes.contains(name), "Mesh cannot be loaded with name \"" + name + "\". Mesh with that name is already present");
			LogInfo("Mesh is loaded. Name: \"" + name + "\"" + " Path: " + fbxPath);
			_meshes.try_emplace(name, _LoadFBX(fbxPath));
		}
		void LoadShader(const std::string& name, const std::string& folderPath) NSL_NOEXCEPT
		{
			NSL_DEBUG
			(
				if (_shaders.contains(name)) LogError("Shader cannot be loaded with name \"" + name + "\". Shader with that name is already present");
				else LogInfo("Shader is loaded. Name: \"" + name + "\"" + " Path: " + folderPath);
			)
			_shaders.try_emplace(name, _LoadShader(folderPath));
		}
		void LoadTexture2D(const std::string& name, const std::string& pngPath) NSL_NOEXCEPT
		{
			NSL_DEBUG
			(
				if (_textures2D.contains(name)) LogError("Texture2D cannot be loaded with name \"" + name + "\". Texture2D with that name is already present");
				else LogInfo("Texture2D is loaded. Name: \"" + name + "\"" + " Path: " + pngPath);
			)
			Texture2D texture = _LoadPNG(pngPath);

			AddTexture2D(name, std::move(texture));
		}
		void ParseTexture2D(const std::string& name, const std::string& pngContent) NSL_NOEXCEPT
		{
			Texture2D texture = _ParsePNG(pngContent);
			LogInfo("Texture2D is parsed. Name: \"" + name + "\"");
			AddTexture2D(name, std::move(texture));
		}

		void AddMesh(const std::string& name, const Mesh& mesh) NSL_NOEXCEPT
		{
			_meshes.try_emplace(name, std::move(mesh));
			LogInfo("Mesh is added. Name: \"" + name + "\"");
		}
		void AddShader(const std::string& name, const Shader& shader) NSL_NOEXCEPT
		{
			_shaders.try_emplace(name, std::move(shader));
			LogInfo("Shader is added. Name: \"" + name + "\"");
		}
		void AddTexture2D(const std::string& name, const Texture2D& texture) NSL_NOEXCEPT
		{
			_textures2D.try_emplace(name, std::move(texture));
			LogInfo("Texture2D is added. Name: \"" + name + "\"");
		}
		void AddShaderStorageBuffer(const std::string& name, const ShaderStorageBuffer& shaderStorageBuffer) NSL_NOEXCEPT
		{
			_shaderStorageBuffers.try_emplace(name, std::move(shaderStorageBuffer));
			LogInfo("ShaderStorageBuffer is added. Name: \"" + name + "\"");
		}
		//void CreateFramebuffer(const std::string& name, const Framebuffer::Data& data) NSL_NOEXCEPT
		//{
		//	_framebuffers.try_emplace(name, data);
		//	LogInfo("Framebuffer is added. Name: \"" + name + "\"");
		//}
		void CreatePixelbuffer(const std::string& name, const Pixelbuffer::Data& data) NSL_NOEXCEPT
		{
			_pixelbuffers.try_emplace(name, data);
			LogInfo("Pixelbuffer is added. Name: \"" + name + "\"");
		}

		Mesh& GetMesh(const std::string& name) NSL_NOEXCEPT
		{
			NSL_DEBUG( if (!_meshes.contains(name)) LogError("Mesh with name \"" + name + "\" is not present"); )
			return _meshes.at(name);
		}
		const Mesh& GetMesh(const std::string& name) const NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_meshes.contains(name)) LogError("Mesh with name \"" + name + "\" is not present"); )
			return _meshes.at(name);
		}
		Texture2D& GetTexture2D(const std::string& name) NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_textures2D.contains(name)) LogError("Texture2D with name \"" + name + "\" is not present"); )
			return _textures2D.at(name);
		}
		const Texture2D& GetTexture2D(const std::string& name) const NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_textures2D.contains(name)) LogError("Texture2D with name \"" + name + "\" is not present"); )
			return _textures2D.at(name);
		}
		Shader& GetShader(const std::string& name) NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_shaders.contains(name)) LogError("Shader with name \"" + name + "\" is not present"); )
			return _shaders.at(name);
		}
		const Shader& GetShader(const std::string& name) const NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_shaders.contains(name)) LogError("Shader with name \"" + name + "\" is not present"); )
			return _shaders.at(name);
		}
		ShaderStorageBuffer& GetShaderStorageBuffer(const std::string& name) NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_shaderStorageBuffers.contains(name)) LogError("ShaderStorageBuffer with name \"" + name + "\" is not present"); )
			return _shaderStorageBuffers.at(name);
		}
		const ShaderStorageBuffer& GetShaderStorageBuffer(const std::string& name) const NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_shaderStorageBuffers.contains(name)) LogError("ShaderStorageBuffer with name \"" + name + "\" is not present"); )
			return _shaderStorageBuffers.at(name);
		}
		//Framebuffer& GetFramebuffer(const std::string& name) NSL_NOEXCEPT
		//{
		//	NSL_DEBUG(if (!_framebuffers.contains(name)) LogError("Framebuffe with name \"" + name + "\" is not present"); )
		//	return _framebuffers[name];
		//}
		Pixelbuffer& GetPixelbuffer(const std::string& name) NSL_NOEXCEPT
		{
			NSL_DEBUG(if (!_pixelbuffers.contains(name)) LogError("Pixelbuffers with name \"" + name + "\" is not present"); )
			return _pixelbuffers[name];
		}

		bool ContainsTexture2D(const std::string& textureName) const NSL_NOEXCEPT
		{
			return _textures2D.contains(textureName);
		}

		void DeleteMesh(const std::string& name) NSL_NOEXCEPT
		{
			_meshes.erase(name);
			LogInfo("Mesh is deleted. Name: \"" + name + "\"");
		}
		void DeleteShader(const std::string& name) NSL_NOEXCEPT
		{
			_shaders.erase(name);
			LogInfo("Shader is deleted. Name: \"" + name + "\"");
		}
		void DeleteTexture2D(const std::string& name) NSL_NOEXCEPT
		{
			_textures2D.erase(name);
			LogInfo("Texture2D is deleted. Name: \"" + name + "\"");
		}
		void DeleteShaderStorageBuffer(const std::string& name) NSL_NOEXCEPT
		{
			_shaderStorageBuffers.erase(name);
			LogInfo("ShaderStorageBuffer is deleted. Name: \"" + name + "\"");
		}
		//void UnloadFramebuffer(const std::string& name) NSL_NOEXCEPT
		//{
		//	_framebuffers.erase(name);
		//	LogInfo("Framebuffer is unloaded. Name: \"" + name + "\"");
		//}
		void DeletePixelbuffer(const std::string& name) NSL_NOEXCEPT
		{
			_pixelbuffers.erase(name);
			LogInfo("Pixelbuffer is deleted. Name: \"" + name + "\"");
		}

	private:
		Shader _LoadShader(const std::string& folderPath) const NSL_NOEXCEPT
		{
			std::string shaderName = NSL::GetNameOfLastFolderInFolderPath(folderPath, '/');

			std::string vertexShaderPath = folderPath + "/" + shaderName + ".vert";
			std::string geometryShaderPath = folderPath + "/" + shaderName + ".geom";
			std::string fragmentShaderPath = folderPath + "/" + shaderName + ".frag";

			std::string vertexShaderText;
			std::string geometryShaderText;
			std::string fragmentShaderText;

			// Creating Shaders
			if (!vertexShaderPath.empty())
			{
				vertexShaderText = NSL::ReadFile(vertexShaderPath);

				// Sometimes for unknown reason shader text has some strange chars in a beginnging of a string. Algorythm below cleans it
				if (!vertexShaderText.empty()) vertexShaderText.erase(0, vertexShaderText.find_first_of('#'));
			}
			if (!geometryShaderPath.empty())
			{
				geometryShaderText = NSL::ReadFile(geometryShaderPath);

				// Sometimes for unknown reason shader text has some strange chars in a beginnging of a string. Algorythm below cleans it
				if (!geometryShaderText.empty()) geometryShaderText.erase(0, geometryShaderText.find_first_of('#'));
			}
			if (!fragmentShaderPath.empty())
			{
				fragmentShaderText = NSL::ReadFile(fragmentShaderPath);

				// Sometimes for unknown reason shader text has some strange chars in a beginnging of a string. Algorythm below cleans it
				if (!fragmentShaderText.empty()) fragmentShaderText.erase(0, fragmentShaderText.find_first_of('#'));
			}

			return Shader(vertexShaderText, geometryShaderText, fragmentShaderText);
		}
		Texture2D _LoadPNG(const std::string& filePath) const NSL_NOEXCEPT
		{
			NSL::PNG png = NSL::PNG::Load(filePath);

			Texture2D texture;

			texture.width  = png.size.x;
			texture.height = png.size.y;
			texture.pixels = png.GetPixelsData();

			switch (png.channels)
			{
			case NSL::PNG::Channels::Red:
				texture.channels = Texture2D::RED;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			case NSL::PNG::Channels::RG:
				texture.channels = Texture2D::RG;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			case NSL::PNG::Channels::RGB:
				texture.channels = Texture2D::RGB;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			case NSL::PNG::Channels::RGBA:
				texture.channels = Texture2D::RGBA;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			default:
				break;
			}

			return texture;
		}
		Texture2D _ParsePNG(const std::string& pngContent) const NSL_NOEXCEPT
		{
			NSL::PNG png = NSL::PNG::Parse(pngContent);

			Texture2D texture;

			texture.width = png.size.x;
			texture.height = png.size.y;
			texture.pixels = png.GetPixelsData();

			switch (png.channels)
			{
			case NSL::PNG::Channels::Red:
				texture.channels = Texture2D::RED;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			case NSL::PNG::Channels::RG:
				texture.channels = Texture2D::RG;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			case NSL::PNG::Channels::RGB:
				texture.channels = Texture2D::RGB;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			case NSL::PNG::Channels::RGBA:
				texture.channels = Texture2D::RGBA;
				texture.colorSpace = Texture2D::ColorSpace::Linear;
				break;

			default:
				break;
			}

			return texture;
		}

		Mesh _LoadFBX(const std::string& filePath) const NSL_NOEXCEPT
		{
			NSL::FBX fbx = _fbxLoader.Load(filePath);

			Mesh mesh;
			mesh.coordinatesPerVertex = 3;

			mesh.vertices.resize(fbx.vertices.size());
			for (size_t i = 0; i < mesh.vertices.size(); ++i)
			{
				mesh.vertices[i] = static_cast<GLfloat>(fbx.vertices[i]);
			}

			mesh.indices.resize(fbx.indices.size());
			for (size_t i = 0; i < mesh.indices.size(); ++i)
			{
				mesh.indices[i] = static_cast<GLuint>(fbx.indices[i]);
			}

			mesh.uvs.resize(fbx.uvs.size());
			for (size_t i = 0; i < mesh.uvs.size(); ++i)
			{
				mesh.uvs[i] = static_cast<GLfloat>(fbx.uvs[i]);
			}

			mesh.normals.resize(fbx.normals.size());
			for (size_t i = 0; i < mesh.normals.size(); ++i)
			{
				mesh.normals[i] = static_cast<GLfloat>(fbx.normals[i]);
			}

			mesh.attributePointers = { 3, 2, 3 };

			return mesh;
		}

	private:
		//NSL::PNGLoader _pngLoader;
		NSL::FBXLoader _fbxLoader;
		std::unordered_map<std::string, Mesh> _meshes;
		std::unordered_map<std::string, Texture2D> _textures2D;
		std::unordered_map<std::string, Shader> _shaders;
		//std::unordered_map<std::string, Framebuffer> _framebuffers;
		std::unordered_map<std::string, Pixelbuffer> _pixelbuffers;
		std::unordered_map<std::string, ShaderStorageBuffer> _shaderStorageBuffers;
	};
}