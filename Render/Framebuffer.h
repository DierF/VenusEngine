#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace VenusEngine
{
	class Framebuffer
	{
	public:
		Framebuffer()
		{
			glCreateFramebuffers(1, &m_framebuffer);
		}

		~Framebuffer()
		{
			glDeleteFramebuffers(1, &m_framebuffer);
		}

		void bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		}

		void unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void texture2D(GLenum attachment, GLuint textureId)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureId, 0);
		}

		void renderbuffer(GLenum attachment, GLuint renderbuffer)
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer);
		}

	private:
		GLuint m_framebuffer;
	};
}