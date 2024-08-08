#pragma once

#include "Render/ShaderProgram.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Math/MathHeaders.h"

namespace VenusEngine
{
	class WorldAxis
	{
	public:
		WorldAxis()
		{
			m_vertexArray.bind();
			m_vertexBuffer.bind();

			// Vertex data for XYZ axis (large numbers for world coverage)
			float intensity = 0.4f;
			float lineVertices[] =
			{
				// X-axis (Red)
				-10000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, intensity, 0.0f, 0.0f, // Point A (-10000, 0, 0)
				 10000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, intensity, 0.0f, 0.0f, // Point B (10000, 0, 0)
				// Y-axis (Green)
				0.0f, -10000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, intensity, 0.0f, // Point C (0, -10000, 0)
				0.0f,  10000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, intensity, 0.0f, // Point D (0, 10000, 0)
				// Z-axis (Blue)
				0.0f, 0.0f, -10000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, intensity, // Point E (0, 0, -10000)
				0.0f, 0.0f,  10000.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, intensity  // Point F (0, 0, 10000)
			};

			m_vertexBuffer.bufferData(sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

			// position, normal, color
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(0));

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
			
			m_vertexBuffer.unbind();
			m_vertexArray.unbind();
		}

		// draw world axis
		// call this after camera draw and before light draw
		void draw(ShaderProgram& shaderProgram)
		{
			shaderProgram.enable();

			shaderProgram.setUniformInt("uNumLights", 0);
			shaderProgram.setUniformMat4("uWorld", Mat4::IDENTITY);

			m_vertexArray.bind();
			glDrawArrays(GL_LINES, 0, 6);
			m_vertexArray.unbind();

			shaderProgram.disable();
		}

	private:
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexBuffer;
	};
}