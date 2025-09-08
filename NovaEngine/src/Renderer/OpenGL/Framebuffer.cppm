export module Framebuffer;
#include "Core.h"
import std;
import NSL;
import Texture2D;

export namespace Renderer
{
	class NSL_API Framebuffer final
	{
	public:
		struct NSL_API Data
		{
			Data() NSL_NOEXCEPT
				: textureName()
				, texture(nullptr)
			{
			}
			Data(const std::string& textureName, const Texture2D* texture) NSL_NOEXCEPT
				: textureName(textureName)
				, texture(texture)
			{
			}

			std::string textureName;
			const Texture2D* texture;
		};

	public:
		Framebuffer() NSL_NOEXCEPT
			: _framebufferID(0)
			, _texture()
			, _renderbufferID(0)
		{
			GLCall(glGenFramebuffers(1, &_framebufferID));
			GLCall(glGenRenderbuffers(1, &_renderbufferID));
		}
		Framebuffer(const Data& data) NSL_NOEXCEPT
			: _framebufferID(0)
			, _texture()
			, _renderbufferID(0)
		{
			_texture = data.textureName;

			GLCall(glGenFramebuffers(1, &_framebufferID));
			GLCall(glGenRenderbuffers(1, &_renderbufferID));

			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, data.texture->GetID(), 0));
			GLCall(glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferID));
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, data.texture->GetWidth(), data.texture->GetHeight()));
			GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbufferID));

			NSL_DEBUG
			(
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					LogError("Framebuffer is not complete!");
				}
			)
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		}
		~Framebuffer() NSL_NOEXCEPT
		{
			glDeleteRenderbuffers(1, &_renderbufferID);
			glDeleteFramebuffers(1, &_framebufferID);
		}

	public:
		GLuint GetFramebufferID() const NSL_NOEXCEPT
		{
			return _framebufferID;
		}
		const std::string& GetTextureName() const NSL_NOEXCEPT
		{
			return _texture;
		}
		void ReloadTexture(const std::string& textureName, const Texture2D* texture)
		{
			_texture = textureName;

			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0));
			GLCall(glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferID));
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture->GetWidth(), texture->GetHeight()));
			GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbufferID));

			NSL_DEBUG
			(
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					LogError("Framebuffer is not complete!");
				}
			)
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		}

	private:
		GLuint _framebufferID;
		GLuint _renderbufferID;
		std::string _texture;
	};
}