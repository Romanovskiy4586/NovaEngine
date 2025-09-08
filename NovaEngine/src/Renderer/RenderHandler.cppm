export module RenderHandler;
#include "Core.h"
import std;
import NSL;
import Camera;
import AssetsManager;
import ResourcesManager;
import GLGlobalContextManager;
import GLTexturesContextManager;
import GLShadersContextManager;
import GLBuffersContextManager;
import GLContext;
import Model;
import Scene;
import Texture2D;
import Shader;
import Mesh;
import Sprite;
import ShaderStorageBuffer;
//import Framebuffer;
import ShadersSourceCodeDB;
import MeshesDataDB;

import ResourcesDB;


const std::string s_screenspaceMeshName = "RENDERHANDLER__screenspaceFace";
const std::string s_spriteMeshName = "RENDERHNDLER__sprite";
const std::string s_spriteBitmapMeshName = "RENDERHNDLER__ spriteBitmap";

const std::string s_framebufferTextureName1 = "FRAMEBUFFER_TEXTURE__PostProcess1";
const std::string s_framebufferTextureName2 = "FRAMEBUFFER_TEXTURE__PostProcess2";
const std::string s_framebufferName1 = "FRAMEBUFFER__PostProcess1";
const std::string s_framebufferName2 = "FRAMEBUFFER__PostProcess2";

const std::string s_spriteShaderName = "RENDERHANDLER__Sprite";
const std::string s_modelShaderName = "RENDERHANDLER__Model";
const std::string s_modelPS1ShaderName = "RENDERHANDLER__ModelPS1";
const std::string s_spriteInstanceOffsetsShaderName = "RENDERHANDLER__SpriteInstanceOffsets";

const std::string s_defaultAlbedoTextureName = "RENDERHANDLER__defaultAlbedoTexture";
const std::string s_defaultNormalTextureName = "RENDERHANDLER__defaultNormalTexture";
const std::string s_defaultMetallicTextureName = "RENDERHANDLER__defaultMetallicTexture";
const std::string s_defaultAOTextureName = "RENDERHANDLER__defaultAOTexture";
const std::string s_defaultRoughnessTextureName = "RENDERHANDLER__defaultRoughnessTextureName";

export namespace Renderer
{
	using Texture2DFiltering = GLTexturesContextManager::Filtering;

	class NSL_API RenderHandler final
		: public NSL::INonCopyable
		, public NSL::INonMovable
	{
	public:
		struct NSL_API PostProcessParameters
		{
			PostProcessParameters(float exposure = 1.0f, float gamma = 2.4f, float kernelOffset = 0.0f,
				NSL::Matrix3x3 kernelMatrix = NSL::Matrix3x3::Zero) NSL_NOEXCEPT
				: exposure(exposure)
				, gamma(gamma)
				, kernelOffset(kernelOffset)
				, kernelMatrix(kernelMatrix)
			{
			}

			float exposure;
			float gamma;
			float kernelOffset;
			NSL::Matrix3x3 kernelMatrix;
		};

	public:
		RenderHandler(int width, int height) NSL_NOEXCEPT
			: glGlobalContextManager(width, height)
			, resourcesManager()
			, assetsManager()
			, postProcessParameters()
			, _scale(1.0f)
			, _framebufferWidth(width)
			, _framebufferHeight(height)
			, _scaledWidth(width)
			, _scaledHeight(height)
		{
			AddMesh(s_screenspaceMeshName, _meshesDataDB.screenspaceSprite);
			AddMesh(s_spriteMeshName, _meshesDataDB.sprite);

			Shader shader;
			shader.vertexSourceCode = _shadersSourceCodeDB.positionUVsPVMPS1VertexShader;
			shader.fragmentSourceCode = _shadersSourceCodeDB.textureMappingFragmentShader;
			AddShader(s_modelPS1ShaderName, shader);


			NovaResources::Shaders_SpriteInstanced_vert vert;
			NovaResources::Shaders_SpriteInstanced_frag frag;

			shader.vertexSourceCode = NSL::ToString(vert.data, vert.size);
			shader.fragmentSourceCode = NSL::ToString(frag.data, frag.size);
			//AddShader(s_spriteShaderName, shader);
			//AddShader(s_modelShaderName, shader);

			//shader.vertexSourceCode = _shadersSourceCodeDB.positionUVsPVMInstanceOffsetsVertexShader;
			AddShader(s_spriteInstanceOffsetsShaderName, shader);

			shader.vertexSourceCode = _shadersSourceCodeDB.postProcessVertexShader;
			shader.fragmentSourceCode = _shadersSourceCodeDB.postProcessFragmentShader_Empty;
			AddShader("Empty", shader);

			shader.fragmentSourceCode = _shadersSourceCodeDB.postProcessFragmentShader_GammaCorrection;
			AddShader("GammaCorrection", shader);

			shader.fragmentSourceCode = _shadersSourceCodeDB.postProcessFragmentShader_TonalCompression;
			AddShader("TonalCompression", shader);

			shader.fragmentSourceCode = _shadersSourceCodeDB.postProcessFragmentShader_Kernel;
			AddShader("Kernel", shader);

			shader.fragmentSourceCode = _shadersSourceCodeDB.postProcessFragmentShader_Sharpener;
			AddShader("Sharpener", shader);

			Texture2D texture;
			AddTexture2D(s_defaultAlbedoTextureName, texture);
			AddTexture2D(s_defaultNormalTextureName, texture);
			AddTexture2D(s_defaultMetallicTextureName, texture);
			AddTexture2D(s_defaultAOTextureName, texture);
			AddTexture2D(s_defaultRoughnessTextureName, texture);
			//resourcesManager.CreateTexture2D(s_framebufferTextureName1, Texture2D::Data(_scaledWidth, _scaledHeight, Texture2D::Data::Channels::RGBA, Texture2D::Data::ColorSpace::Linear, std::vector<unsigned char>(), false, false));
			//resourcesManager.CreateTexture2D(s_framebufferTextureName2, Texture2D::Data(_scaledWidth, _scaledHeight, Texture2D::Data::Channels::RGBA, Texture2D::Data::ColorSpace::Linear, std::vector<unsigned char>(), false, false));
			//resourcesManager.CreateFramebuffer(s_framebufferName1, Framebuffer::Data(s_framebufferTextureName1, &resourcesManager.GetTexture2D(s_framebufferTextureName1)));
			//resourcesManager.CreateFramebuffer(s_framebufferName2, Framebuffer::Data(s_framebufferTextureName2, &resourcesManager.GetTexture2D(s_framebufferTextureName2)));
		}
		~RenderHandler() NSL_NOEXCEPT
		{
			DeleteMesh(s_screenspaceMeshName);
			DeleteMesh(s_spriteMeshName);
			
			//DeleteShader(s_spriteShaderName);
			//DeleteShader(s_modelShaderName);
			DeleteShader(s_modelPS1ShaderName);
			DeleteShader(s_spriteInstanceOffsetsShaderName);
			DeleteShader("Empty");
			DeleteShader("GammaCorrection");
			DeleteShader("TonalCompression");
			DeleteShader("Kernel");
			DeleteShader("Sharpener");

			DeleteTexture2D(s_defaultAlbedoTextureName);
			DeleteTexture2D(s_defaultNormalTextureName);
			DeleteTexture2D(s_defaultMetallicTextureName);
			DeleteTexture2D(s_defaultAOTextureName);
			DeleteTexture2D(s_defaultRoughnessTextureName);
		}

	public: // Shaders
		void AddShader(const std::string& name, const Shader& shader) NSL_NOEXCEPT
		{
			resourcesManager.AddShader(name, shader);
			glShadersContextManager.RegisterShader(resourcesManager.GetShader(name));
		}
		void BindShader(const std::string& name) NSL_NOEXCEPT
		{
			glShadersContextManager.BindShader(&resourcesManager.GetShader(name));
		}
		void SetShaderUniform(const NSL::Vector2& data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(const std::vector<NSL::Vector2> data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(const NSL::Vector3& data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(const NSL::Vector4& data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(int data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(bool data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(float data, const std::string& uniformName) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName);
		}
		void SetShaderUniform(const NSL::Matrix4x4& data, const std::string& uniformName, bool transpose) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName, transpose);
		}
		void SetShaderUniform(const NSL::Matrix3x3& data, const std::string& uniformName, bool transpose) NSL_NOEXCEPT
		{
			glShadersContextManager.SetUniform(data, uniformName, transpose);
		}
		void DeleteShader(const std::string& name) NSL_NOEXCEPT
		{
			glShadersContextManager.FreeShader(resourcesManager.GetShader(name));
			resourcesManager.DeleteShader(name);
		}

	public: // Textures
		void AddTexture2D(const std::string& name, const Texture2D& texture, Texture2DFiltering filtering = Texture2DFiltering::MipmapNearest) NSL_NOEXCEPT
		{
			resourcesManager.AddTexture2D(name, texture);
			Texture2D& textureRef = resourcesManager.GetTexture2D(name);
			glTexturesContextManager.RegisterTexture2D(textureRef, filtering);
			glTexturesContextManager.UpdateTexture2D(textureRef);
		}
		void LoadTexture2D(const std::string& name, const std::string& pngPath, Texture2DFiltering filtering = Texture2DFiltering::MipmapNearest) NSL_NOEXCEPT
		{
			resourcesManager.LoadTexture2D(name, pngPath);
			Texture2D& textureRef = resourcesManager.GetTexture2D(name);
			glTexturesContextManager.RegisterTexture2D(textureRef, filtering);
			glTexturesContextManager.UpdateTexture2D(textureRef);
		}
		void BindTexture2D(const std::string& name) NSL_NOEXCEPT
		{
			glTexturesContextManager.BindTexture2D(&resourcesManager.GetTexture2D(name));
		}
		const Texture2D& GetTexture2D(const std::string& name) const NSL_NOEXCEPT
		{
			return resourcesManager.GetTexture2D(name);
		}
		Texture2D& GetTexture2D(const std::string& name) NSL_NOEXCEPT
		{
			return resourcesManager.GetTexture2D(name);
		}
		void UpdateTexture2D(const std::string& name) NSL_NOEXCEPT
		{
			glTexturesContextManager.UpdateTexture2D(resourcesManager.GetTexture2D(name));
		}
		void DeleteTexture2D(const std::string& name) NSL_NOEXCEPT
		{
			glTexturesContextManager.FreeTexture2D(resourcesManager.GetTexture2D(name));
			resourcesManager.DeleteTexture2D(name);
		}

	public:
		void AddMesh(const std::string& meshName, const Mesh& mesh) NSL_NOEXCEPT
		{
			resourcesManager.AddMesh(meshName, mesh);
			glBuffersContextManager.RegisterMesh(resourcesManager.GetMesh(meshName));
		}
		void LoadMesh(const std::string& meshName, const std::string& pngPath) NSL_NOEXCEPT
		{
			resourcesManager.LoadMesh(meshName, pngPath);
			glBuffersContextManager.RegisterMesh(resourcesManager.GetMesh(meshName));
		}
		void DeleteMesh(const std::string& meshName) NSL_NOEXCEPT
		{
			glBuffersContextManager.FreeMesh(resourcesManager.GetMesh(meshName));
			resourcesManager.DeleteMesh(meshName);
		}
		void DrawMesh(const std::string& meshName, unsigned int instanceCount = 1) NSL_NOEXCEPT
		{
			glBuffersContextManager.DrawCall(resourcesManager.GetMesh(meshName), instanceCount);
		}
		void DrawMesh(const Model& model, unsigned int instanceCount = 1) NSL_NOEXCEPT
		{
			glBuffersContextManager.DrawCall(model.mesh, instanceCount);
		}

	public:
		void CreateShaderStorageBuffer(const std::string& name) NSL_NOEXCEPT
		{
			resourcesManager.AddShaderStorageBuffer(name, ShaderStorageBuffer());
			glBuffersContextManager.RegisterShaderStorageBuffer(resourcesManager.GetShaderStorageBuffer(name));
		}
		void AddShaderStorageBuffer(const std::string& name, const ShaderStorageBuffer& shaderStorageBuffer) NSL_NOEXCEPT
		{
			resourcesManager.AddShaderStorageBuffer(name, shaderStorageBuffer);
			glBuffersContextManager.RegisterShaderStorageBuffer(resourcesManager.GetShaderStorageBuffer(name));
		}
		template <class T>
		void UpdateShaderStorageBuffer(const std::string& name, const std::vector<T> data) NSL_NOEXCEPT
		{
			resourcesManager.GetShaderStorageBuffer(name).Reallocate(data.data(), static_cast<unsigned long long>(data.size() * sizeof(T)), static_cast<unsigned int>(data.size()));
			glBuffersContextManager.ReloadShaderStorageBuffer(resourcesManager.GetShaderStorageBuffer(name));
		}
		void DeleteShaderStorageBuffer(const std::string& name) NSL_NOEXCEPT
		{
			glBuffersContextManager.FreeShaderStorageBuffer(resourcesManager.GetShaderStorageBuffer(name));
			resourcesManager.DeleteShaderStorageBuffer(name);
		}

	public:
		void Clear()
		{
			glGlobalContextManager.Clear();
		}
		void SetClearColor(const NSL::Vector4& color) NSL_NOEXCEPT
		{
			glGlobalContextManager.SetClearColor(color);
		}
		void SetViewport(int width, int height) NSL_NOEXCEPT
		{
			glGlobalContextManager.SetViewport(width, height);
		}
		void SetViewport(int x, int y, int width, int height) NSL_NOEXCEPT
		{
			glGlobalContextManager.SetViewport(x, y, width, height);
		}

		void UpdateFramebufferSize(int width, int height) NSL_NOEXCEPT
		{
			_framebufferWidth = width;
			_framebufferHeight = height;
			_scaledWidth = static_cast<int>(_framebufferWidth * _scale);
			_scaledHeight = static_cast<int>(_framebufferHeight * _scale);

			//auto& texture1 = resourcesManager.GetTexture2D(s_framebufferTextureName1);
			//auto& texture2 = resourcesManager.GetTexture2D(s_framebufferTextureName2);

			//texture1.SetSize(_scaledWidth, _scaledHeight);
			//texture2.SetSize(_scaledWidth, _scaledHeight);

			//texture1.ReloadOnGPU();
			//texture2.ReloadOnGPU();

			//resourcesManager.GetFramebuffer(s_framebufferName1).ReloadTexture(s_framebufferTextureName1, &texture1);
			//resourcesManager.GetFramebuffer(s_framebufferName2).ReloadTexture(s_framebufferTextureName2, &texture2);
		}
		void SetScale(float scale) NSL_NOEXCEPT
		{
			_scale = scale;
			_scaledWidth = static_cast<int>(_framebufferWidth * _scale);
			_scaledHeight = static_cast<int>(_framebufferHeight * _scale);

			UpdateFramebufferSize(_framebufferWidth, _framebufferHeight);
		}

	public:
		void CreateSprite(const std::string& spriteName, const std::string& textureName) NSL_NOEXCEPT
		{
			assetsManager.BindIntoSprite(spriteName, resourcesManager.GetTexture2D(textureName));
		}
		Sprite& GetSprite(const std::string& name) NSL_NOEXCEPT
		{
			return assetsManager.GetSprite(name);
		}
		const Sprite& GetSprite(const std::string& name) const NSL_NOEXCEPT
		{
			return assetsManager.GetSprite(name);
		}

		void CreateModel(const std::string& modelName, const std::string& meshName,
			const std::string& albedoTextureName, const std::string& normalTextureName,
			const std::string& metallicTextureName, const std::string& aoTextureName,
			const std::string& roughnessTextureName) NSL_NOEXCEPT
		{
			Mesh& mesh = resourcesManager.GetMesh(meshName);

			Texture2D& albedoTexture = resourcesManager.ContainsTexture2D(albedoTextureName)
				? resourcesManager.GetTexture2D(albedoTextureName)
				: resourcesManager.GetTexture2D(s_defaultAlbedoTextureName);

			Texture2D& normalTexture = resourcesManager.ContainsTexture2D(normalTextureName)
				? resourcesManager.GetTexture2D(normalTextureName)
				: resourcesManager.GetTexture2D(s_defaultNormalTextureName);

			Texture2D& metallicTexture = resourcesManager.ContainsTexture2D(metallicTextureName)
				? resourcesManager.GetTexture2D(metallicTextureName)
				: resourcesManager.GetTexture2D(s_defaultMetallicTextureName);

			Texture2D& aoTexture = resourcesManager.ContainsTexture2D(aoTextureName)
				? resourcesManager.GetTexture2D(aoTextureName)
				: resourcesManager.GetTexture2D(s_defaultAOTextureName);

			Texture2D& roughnessTexture = resourcesManager.ContainsTexture2D(roughnessTextureName)
				? resourcesManager.GetTexture2D(roughnessTextureName)
				: resourcesManager.GetTexture2D(s_defaultRoughnessTextureName);

			assetsManager.BindIntoModel(modelName, mesh, albedoTexture, normalTexture, metallicTexture, aoTexture, roughnessTexture);
		}
		Model& GetModel(const std::string& name) NSL_NOEXCEPT
		{
			return assetsManager.GetModel(name);
		}
		const Model& GetModel(const std::string& name) const NSL_NOEXCEPT
		{
			return assetsManager.GetModel(name);
		}

	public:
		void Render(Scene& scene) NSL_NOEXCEPT
		{
			_Sort(scene);

			BindShader(s_spriteShaderName);

			for (auto& spriteName : scene.sprites)
			{
				Sprite& sprite = assetsManager.GetSprite(spriteName);

				glTexturesContextManager.BindTexture2D(&sprite.texture);
				SetShaderUniform(scene.camera.GetProjectionViewMatrix() * sprite.transform.GetModelMatrix(), "pvm", false);

				DrawMesh(s_spriteMeshName);
			}
		}
		void RenderSprite(const std::string& spriteName, const Camera& camera) NSL_NOEXCEPT
		{
			static const std::string pvmUniformName("pvm");
			Sprite& sprite = assetsManager.GetSprite(spriteName);

			BindShader(s_spriteShaderName);
			glTexturesContextManager.BindTexture2D(&sprite.texture);
			SetShaderUniform(camera.GetProjectionViewMatrix() * sprite.transform.GetModelMatrix(), pvmUniformName, false);
			DrawMesh(s_spriteMeshName);
		}
		void RenderSpriteInstanced(const std::string& spriteName, const Camera& camera, const std::string& shaderStorageNameWithPositions) NSL_NOEXCEPT
		{
			static const std::string pvmUniformName("pvm");
			Sprite& sprite = assetsManager.GetSprite(spriteName);

			BindShader(s_spriteInstanceOffsetsShaderName);
			glTexturesContextManager.BindTexture2D(&sprite.texture);
			SetShaderUniform(camera.GetProjectionViewMatrix() * sprite.transform.GetModelMatrix(), pvmUniformName, false);
			glBuffersContextManager.BindShaderStorageBuffer(&resourcesManager.GetShaderStorageBuffer(shaderStorageNameWithPositions), 0);
			DrawMesh(s_spriteMeshName, resourcesManager.GetShaderStorageBuffer(shaderStorageNameWithPositions).Count());
		}

		void RenderModel(const std::string& modelName, const Camera& camera) NSL_NOEXCEPT
		{
			static const std::string pvmUniformName("pvm");
			Model& model = assetsManager.GetModel(modelName);

			BindShader(s_modelShaderName);
			glTexturesContextManager.BindTexture2D(&model.albedo, 0);
			glTexturesContextManager.BindTexture2D(&model.normal, 1);
			glTexturesContextManager.BindTexture2D(&model.metallic, 2);
			glTexturesContextManager.BindTexture2D(&model.ao, 3);
			glTexturesContextManager.BindTexture2D(&model.roughness, 4);
			SetShaderUniform(camera.GetProjectionViewMatrix() * model.transform.GetModelMatrix(), pvmUniformName, false);
			DrawMesh(model);
		}
		void RenderModelPS1(const std::string& modelName, const Camera& camera) NSL_NOEXCEPT
		{
			static const std::string pvmUniformName("pvm");
			Model& model = assetsManager.GetModel(modelName);

			BindShader(s_modelPS1ShaderName);
			glTexturesContextManager.BindTexture2D(&model.albedo, 0);
			glTexturesContextManager.BindTexture2D(&model.normal, 1);
			glTexturesContextManager.BindTexture2D(&model.metallic, 2);
			glTexturesContextManager.BindTexture2D(&model.ao, 3);
			glTexturesContextManager.BindTexture2D(&model.roughness, 4);
			SetShaderUniform(camera.GetProjectionViewMatrix() * model.transform.GetModelMatrix(), pvmUniformName, false);
			DrawMesh(model);
		}

	public:
		PostProcessParameters postProcessParameters;
		GLGlobalContextManager glGlobalContextManager;
		GLTexturesContextManager glTexturesContextManager;
		GLShadersContextManager glShadersContextManager;
		GLBuffersContextManager glBuffersContextManager;
		ResourcesManager resourcesManager;
		AssetsManager assetsManager;

	private:
		void _Sort(Scene& scene) NSL_NOEXCEPT
		{
			switch (scene.renderParameters.sortingByDistance)
			{
			case Scene::RenderParameters::SortingByDistance::None:
				break;

			case Scene::RenderParameters::SortingByDistance::NearToFar:
				std::sort(scene.sprites.begin(), scene.sprites.end(), [&](const std::string& a, const std::string& b)
					{
						float distanceA = NSL::Distance(scene.camera.GetPosition(), assetsManager.GetSprite(a).transform.GetPosition());
						float distanceB = NSL::Distance(scene.camera.GetPosition(), assetsManager.GetSprite(b).transform.GetPosition());
						return distanceA < distanceB;
					});
				break;

			case Scene::RenderParameters::SortingByDistance::FarToNear:
				std::sort(scene.sprites.begin(), scene.sprites.end(), [&](const std::string& a, const std::string& b)
					{
						float distanceA = NSL::Distance(scene.camera.GetPosition(), assetsManager.GetSprite(a).transform.GetPosition());
						float distanceB = NSL::Distance(scene.camera.GetPosition(), assetsManager.GetSprite(b).transform.GetPosition());
						return distanceA > distanceB;
					});
				break;

			default:
				LogError("Unknown Sorting parameter");
				break;
			}
		}

	private:
		//NSL::ResourceDB _resourceDB;
		ShadersSourceCodeDB _shadersSourceCodeDB;
		MeshesDataDB _meshesDataDB;
		float _scale;
		int _framebufferWidth;
		int _framebufferHeight;
		int _scaledWidth;
		int _scaledHeight;
	};
}