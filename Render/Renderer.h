#pragma once

#include <glad/glad.h>

#include "Render/ShaderProgram.h"

namespace VenusEngine
{
	class Renderer
	{
	public:
		Renderer()
		{
			m_shaderProgram.createVertexShader("../Render/Vec2.vert");
			m_shaderProgram.createFragmentShader("../Render/Vec2.frag");
			m_shaderProgram.link();
		}

		~Renderer() = default;

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