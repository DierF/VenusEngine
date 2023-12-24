#pragma once

#include <vector>

#include "Render/Renderer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"

namespace VenusEngine
{
	class Scene
	{
	public:
		Scene()
		{
			// Set up triangle geometry
			m_vao1.bind();
			m_vbo1.bind();
			// 3 2D points, followed by 3 RGB colors
			std::vector<float> triVertices{
				0.0f, 0.5f,         // 2-d coordinates of first vertex(X, Y)
				-0.5f, -0.5f,       // 2-d coordinates of second vertex(X, Y)
				0.5f, -0.5f,        // 2-d coordinates of third vertex(X, Y)
				1.0f, 0.0f, 0.0f,   // color of first vertex(R, G, B)
				0.0f, 1.0f, 0.0f,   // color of second vertex(R, G, B)
				0.0f, 0.0f, 1.0f    // color of third vertex(R, G, B)
			};
			m_vbo1.bufferData(triVertices.size() * sizeof(float), triVertices.data(), GL_STATIC_DRAW);
			// Tell the shaders how the data in the array is laid out
			glEnableVertexAttribArray(0);
			// Positions have 2 parts, each are floats, and start at beginning of array
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,
				reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(1);
			// Colors have 3 parts, each are floats, and start at 7th position in array
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
				reinterpret_cast<void*>(6 * sizeof(float)));
			m_vbo1.unbind();
			m_vao1.unbind();

			///////////////////////////////////////////////////////////////////
			// Use a second VAO for point geometry
			glPointSize(50);
			m_vao2.bind();
			m_vbo2.bind();
			// 1 2D point, followed by 1 RGB color
			float pointVertices[] = {
			  0.8f, 0.8f,         // 2-d coordinates of only vertex(X, Y)
			  1.0f, 0.0f, 1.0f    // color of only vertex(R, G, B)
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), pointVertices,
				GL_STATIC_DRAW);
			m_vbo2.bufferData(sizeof(pointVertices), pointVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			// Positions have 2 parts, each are floats, and start at beginning of aray
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,
				reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(1);
			// Colors have 3 parts, each are floats, and start at 3rd position in array
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
				reinterpret_cast<void*>(2 * sizeof(float)));
			m_vbo2.unbind();
			m_vao2.unbind();
		}

		~Scene() = default;

		void draw()
		{
			m_renderer.clearBuffer();
			m_renderer.getShaderProgram().enable();

			m_vao1.bind();
			glDrawArrays(GL_TRIANGLES, 0, 3);
			m_vao1.unbind();

			m_vao2.bind();
			glDrawArrays(GL_POINTS, 0, 1);
			m_vao2.unbind();

			m_renderer.getShaderProgram().disable();
		}

	private:
		Renderer m_renderer;
		VertexArray m_vao1;
		VertexArray m_vao2;
		VertexBuffer m_vbo1;
		VertexBuffer m_vbo2;
	};
}