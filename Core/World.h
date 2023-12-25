#pragma once

#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Render/Renderer.h"

namespace VenusEngine
{
	class World
	{
	public:
		World()
			: m_camera(glm::vec3(0.0f, 0.0f, 15.0f), 0.0f, 0.0f, 0.01f, 40.0f, 1200.0f / 900.0f, 45.0f)
		{
			// 3 2D points, followed by 3 RGB colors
			std::vector<float> triVertices
			{
				0.0f, 5.0f, 0.0f,   // 3-d coordinates of first vertex(X, Y, Z)
				1.0f, 0.0f, 0.0f,   // color of first vertex(R, G, B)
				-5.0f, -5.0f, 0.0f, // 3-d coordinates of second vertex(X, Y, Z)
				0.0f, 1.0f, 0.0f,   // color of second vertex(R, G, B)
				5.0f, -5.0f, 0.0f,  // 3-d coordinates of third vertex(X, Y, Z)
				0.0f, 0.0f, 1.0f    // color of third vertex(R, G, B)
			};
			std::shared_ptr<Mesh> mesh_ptr(new Mesh());
			mesh_ptr->addGeometry(triVertices);
			mesh_ptr->prepareVao();
			m_scene.add("Triangle", mesh_ptr);
			//////////////////////////////////////////////////////////////////////
			m_renderer.getShaderProgram().enable();
			m_renderer.getShaderProgram().setUniformMatrix("uProjection", m_camera.getProjectionMatrix());
			m_renderer.getShaderProgram().disable();
		}

		void draw()
		{
			m_renderer.clearBuffer();
			m_scene.draw(m_renderer.getShaderProgram(), m_camera.getViewMatrix());
		}

	private:
		Renderer m_renderer;
		Camera m_camera;
		Scene m_scene;
	};
}