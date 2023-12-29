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

		void texture(GLuint textureId)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
		}

	private:
		GLuint m_framebuffer;
	};
}