#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace VenusEngine
{
	class Renderbuffer
	{
	public:
		Renderbuffer()
		{
			glGenRenderbuffers(1, &m_renderbuffer);
		}

		~Renderbuffer()
		{
			glDeleteRenderbuffers(1, &m_renderbuffer);
		}

		void bind()
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
		}

		void unbind()
		{
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		void storage(GLsizei width, GLsizei height)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		}

		// reallocate storage by deleting and recreating
		void resize(GLsizei width, GLsizei height)
		{
			glDeleteRenderbuffers(1, &m_renderbuffer);
			glGenRenderbuffers(1, &m_renderbuffer);
			bind();
			storage(width, height);
		}

		GLuint id()
		{
			return m_renderbuffer;
		}

	private:
		GLuint m_renderbuffer;
	};
}