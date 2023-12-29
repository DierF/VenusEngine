#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace VenusEngine
{
	class Texture
	{
	public:
		Texture()
		{
			glGenTextures(1, &m_texture);
		}

		~Texture()
		{
			glDeleteTextures(1, &m_texture);
		}

		void bind()
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void image2D(GLsizei width, GLsizei height)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}

		void filter()
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		GLuint id()
		{
			return m_texture;
		}

	private:
		GLuint m_texture;
	};
}