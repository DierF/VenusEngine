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

		void image2D(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, nullptr);
		}

		void filter(GLint param)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		}

		GLuint id()
		{
			return m_texture;
		}

	private:
		GLuint m_texture;
	};
}