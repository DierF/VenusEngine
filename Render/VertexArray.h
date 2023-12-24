#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace VenusEngine
{
	class VertexArray
	{
	public:
		VertexArray()
		{
			glGenVertexArrays(1, &m_vertexArray);
		}

		~VertexArray()
		{
			glDeleteVertexArrays(1, &m_vertexArray);
		}

		void bind()
		{
			glBindVertexArray(m_vertexArray);
		}

		void unbind()
		{
			glBindVertexArray(0);
		}

	private:
		GLuint m_vertexArray;
	};
}