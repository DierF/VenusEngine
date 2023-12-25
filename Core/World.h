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
		{
			// 3 2D points, followed by 3 RGB colors
			std::vector<float> triVertices{
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
			// Define the projection, which will remain constant
			constexpr float verticalFov = glm::radians(50.0f);
			float aspectRatio = 1200.0f / 900;
			// Near plane
			float nearZ = 0.01f;
			// Far plane
			float farZ = 40.0f;
			m_camera.projection = glm::perspective(verticalFov, aspectRatio, nearZ, farZ);
			// Enable shader program so we can set uniforms
			m_renderer.getShaderProgram().enable();
			m_renderer.getShaderProgram().setUniformMatrix("uProjection", m_camera.projection);
			m_renderer.getShaderProgram().disable();
			// Define the camera's coordinate system
			m_camera.position = glm::vec3(0, 0, 12.0f);
			m_camera.at = glm::vec3(0, 0, 0);
			m_camera.up = glm::vec3(0, 1, 0);
		}

		void draw()
		{
			m_renderer.clearBuffer();
			glm::mat4 modelView = glm::lookAt(m_camera.position, m_camera.at, m_camera.up);
			m_scene.draw(m_renderer.getShaderProgram(), modelView);
		}

	private:
		Renderer m_renderer;
		Camera m_camera;
		Scene m_scene;
	};
}