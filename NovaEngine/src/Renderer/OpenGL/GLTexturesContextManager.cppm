export module GLTexturesContextManager;
#include "Core.h"
#include "glad.h"
import std;
import NSL;
import Texture2D;

export namespace Renderer
{
	export class NSL_API GLTexturesContextManager final
		: public NSL::INonCopyable
		, public NSL::INonMovable
	{
	public:
		enum class NSL_API Filtering
		{
			Nearest, Linear, MipmapNearest, MipmapLinear
		};

		using Texture2DFiltering = GLTexturesContextManager::Filtering;

	public:
		GLTexturesContextManager()
		{
			SetActiveTextureUnit(0);

			_InitGLTextureUnits();

			for (unsigned int i = 0; i < 32; ++i)
			{
				_bindedTextures[i] = 0;
			}
			LogInfo("GLTexturesContextManager is created");
		}

	public:
		void SetActiveTextureUnit(unsigned int unit) NSL_NOEXCEPT
		{
			NSL_ASSERT(unit < 32, "Texture unit index is out of bounds. Expected value 0 - 31. Input value: " + std::to_string(unit));
			if (_activeTextureUnit == unit) return;
			_activeTextureUnit = unit;
			GLCall(glActiveTexture(_glTextureUnits[unit]));
		}
		void BindTexture2D(const Texture2D* texture, unsigned int unit) NSL_NOEXCEPT
		{
			SetActiveTextureUnit(unit);
			BindTexture2D(texture);
		}
		void BindTexture2D(const Texture2D* texture) NSL_NOEXCEPT
		{
			NSL_ASSERT(texture->_id, "Cannot bind texture, because it's ID is 0")

			if (_bindedTextures[_activeTextureUnit] == texture->_id) return;
			_bindedTextures[_activeTextureUnit] = texture->_id;

			GLCall(glBindTexture(GL_TEXTURE_2D, texture->_id ? texture->_id : 0));
		}
		void RegisterTexture2D(Texture2D& texture, Filtering filtering = Filtering::Nearest) NSL_NOEXCEPT
		{
			GLCall(glGenTextures(1, &texture._id));
			BindTexture2D(&texture);

			int format = _GetGLFormatFromTexture(texture);

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.GetPixels()));
			GLCall(glGenerateTextureMipmap(texture._id));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			SetTexture2DFiltering(texture, filtering);
		}
		void FreeTexture2D(Texture2D& texture) NSL_NOEXCEPT
		{
			GLCall(glDeleteTextures(1, &texture._id));
			texture._id = 0;

			for (auto& tex : _bindedTextures)
			{
				if (tex == texture._id)
				{
					tex = 0;
				}
			}
		}
		void UpdateTexture2D(Texture2D& texture) NSL_NOEXCEPT
		{
			BindTexture2D(&texture);
			int format = _GetGLFormatFromTexture(texture);
			GLCall(glTextureSubImage2D(texture._id, 0, 0, 0, texture.width, texture.height, format, GL_UNSIGNED_BYTE, texture.GetPixels()));
		}
		void SetTexture2DFiltering(const Texture2D& texture, Texture2DFiltering filtering) NSL_NOEXCEPT
		{
			BindTexture2D(&texture);

			int minFilter;
			int magFilter;

			switch (filtering)
			{
			case Texture2DFiltering::Nearest:
				minFilter = GL_NEAREST;
				magFilter = GL_NEAREST;
				break;

			case Texture2DFiltering::Linear:
				minFilter = GL_LINEAR;
				magFilter = GL_LINEAR;
				break;

			case Texture2DFiltering::MipmapNearest:
				minFilter = GL_NEAREST_MIPMAP_NEAREST;
				magFilter = GL_NEAREST;
				break;

			case Texture2DFiltering::MipmapLinear:
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
				magFilter = GL_LINEAR;
				break;

			default:
				LogError("Unknown Filtering parameter");
				break;
			}

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
		}

	private:
		void _InitGLTextureUnits() NSL_NOEXCEPT
		{
			_glTextureUnits[0] = GL_TEXTURE0;
			_glTextureUnits[1] = GL_TEXTURE1;
			_glTextureUnits[2] = GL_TEXTURE2;
			_glTextureUnits[3] = GL_TEXTURE3;
			_glTextureUnits[4] = GL_TEXTURE4;
			_glTextureUnits[5] = GL_TEXTURE5;
			_glTextureUnits[6] = GL_TEXTURE6;
			_glTextureUnits[7] = GL_TEXTURE7;
			_glTextureUnits[8] = GL_TEXTURE8;
			_glTextureUnits[9] = GL_TEXTURE9;
			_glTextureUnits[10] = GL_TEXTURE10;
			_glTextureUnits[11] = GL_TEXTURE11;
			_glTextureUnits[12] = GL_TEXTURE12;
			_glTextureUnits[13] = GL_TEXTURE13;
			_glTextureUnits[14] = GL_TEXTURE14;
			_glTextureUnits[15] = GL_TEXTURE15;
			_glTextureUnits[16] = GL_TEXTURE16;
			_glTextureUnits[17] = GL_TEXTURE17;
			_glTextureUnits[18] = GL_TEXTURE18;
			_glTextureUnits[19] = GL_TEXTURE19;
			_glTextureUnits[20] = GL_TEXTURE20;
			_glTextureUnits[21] = GL_TEXTURE21;
			_glTextureUnits[22] = GL_TEXTURE22;
			_glTextureUnits[23] = GL_TEXTURE23;
			_glTextureUnits[24] = GL_TEXTURE24;
			_glTextureUnits[25] = GL_TEXTURE25;
			_glTextureUnits[26] = GL_TEXTURE26;
			_glTextureUnits[27] = GL_TEXTURE27;
			_glTextureUnits[28] = GL_TEXTURE28;
			_glTextureUnits[29] = GL_TEXTURE29;
			_glTextureUnits[30] = GL_TEXTURE30;
			_glTextureUnits[31] = GL_TEXTURE31;
		}
		int _GetGLFormatFromTexture(const Texture2D& texture) const NSL_NOEXCEPT
		{
			int format;
			switch (texture.channels)
			{
			case Texture2D::RED:
				format = GL_RED;
				break;

			case Texture2D::RG:
				format = GL_RG;
				break;

			case Texture2D::RGB:
				switch (texture.colorSpace)
				{
				case Texture2D::ColorSpace::Linear:
					format = GL_RGB;
					break;
				case Texture2D::ColorSpace::SRGB:
					format = GL_SRGB;
					break;
				default:
					LogError("Unknown ColorSpace parameter");
					break;
				}
				break;

			case Texture2D::RGBA:
				switch (texture.colorSpace)
				{
				case Texture2D::ColorSpace::Linear:
					format = GL_RGBA;
					break;
				case Texture2D::ColorSpace::SRGB:
					format = GL_SRGB_ALPHA;
					break;
				default:
					LogError("Unknown ColorSpace parameter");
					break;
				}
				break;

			default:
				LogError("Unknown Channels parameter");
				break;
			}

			return format;
		}

	private:
		unsigned int _activeTextureUnit;
		std::array<unsigned int, 32> _glTextureUnits;
		std::array<unsigned int, 32> _bindedTextures;
	};
}