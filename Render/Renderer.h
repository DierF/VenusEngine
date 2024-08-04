#pragma once

#include <glad/glad.h>

#include "Render/ShaderProgram.h"

namespace VenusEngine
{
	class Renderer
	{
	public:
		Renderer(std::string const& vertexShaderPath, std::string const& fragmentShaderPath)
		{
			m_shaderProgram.createVertexShader(vertexShaderPath);
			m_shaderProgram.createFragmentShader(fragmentShaderPath);
			m_shaderProgram.link();
		}

		~Renderer() = default;

		void drawBuffers(GLsizei size, GLenum const* buffers)
		{
			glDrawBuffers(size, buffers);
		}

		void clearBuffer()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		ShaderProgram& getShaderProgram()
		{
			return m_shaderProgram;
		}

	private:
		ShaderProgram m_shaderProgram;
	};
}