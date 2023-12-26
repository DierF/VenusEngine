#pragma once

#include "Editor/Window.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/Controller.h"
#include "Render/Renderer.h"

namespace VenusEngine
{
	class World
	{
	public:
		World()
			: m_camera(glm::vec3(0.0f, 0.0f, 15.0f), 0.0f, 0.0f, 0.1f, 1000.0f, 1200.0f / 900.0f, 45.0f)
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
			std::shared_ptr<Mesh> tri_mesh_ptr(new Mesh());
			tri_mesh_ptr->addGeometry(triVertices);
			tri_mesh_ptr->prepareVao();
			m_scene.add("Triangle", tri_mesh_ptr);

            std::vector<float> cubeVertices;

            // Define the vertices of a cube
            float side = 5.0f; // Length of each side

            // Vertices
            float v0[] = { -side / 2 + 7.0f, -side / 2, -side / 2, 1.0f, 0.0f, 0.0f }; // Red
            float v1[] = {  side / 2 + 7.0f, -side / 2, -side / 2, 1.0f, 1.0f, 0.0f };  // Yellow
            float v2[] = {  side / 2 + 7.0f,  side / 2, -side / 2, 0.0f, 1.0f, 0.0f };   // Green
            float v3[] = { -side / 2 + 7.0f,  side / 2, -side / 2, 0.0f, 0.0f, 1.0f };  // Blue
            float v4[] = { -side / 2 + 7.0f, -side / 2,  side / 2, 1.0f, 1.0f, 0.0f };  // Yellow
            float v5[] = {  side / 2 + 7.0f, -side / 2,  side / 2, 0.0f, 1.0f, 0.0f };   // Green
            float v6[] = {  side / 2 + 7.0f,  side / 2,  side / 2, 0.0f, 0.0f, 1.0f };    // Blue
            float v7[] = { -side / 2 + 7.0f,  side / 2,  side / 2, 1.0f, 0.0f, 0.0f };   // Red

            // Define the cube using two triangles for each face
            // Front face
            cubeVertices.insert(cubeVertices.end(), std::begin(v0), std::end(v0));
            cubeVertices.insert(cubeVertices.end(), std::begin(v1), std::end(v1));
            cubeVertices.insert(cubeVertices.end(), std::begin(v2), std::end(v2));

            cubeVertices.insert(cubeVertices.end(), std::begin(v0), std::end(v0));
            cubeVertices.insert(cubeVertices.end(), std::begin(v2), std::end(v2));
            cubeVertices.insert(cubeVertices.end(), std::begin(v3), std::end(v3));

            // Right face
            cubeVertices.insert(cubeVertices.end(), std::begin(v1), std::end(v1));
            cubeVertices.insert(cubeVertices.end(), std::begin(v5), std::end(v5));
            cubeVertices.insert(cubeVertices.end(), std::begin(v6), std::end(v6));

            cubeVertices.insert(cubeVertices.end(), std::begin(v1), std::end(v1));
            cubeVertices.insert(cubeVertices.end(), std::begin(v6), std::end(v6));
            cubeVertices.insert(cubeVertices.end(), std::begin(v2), std::end(v2));

            // Back face
            cubeVertices.insert(cubeVertices.end(), std::begin(v5), std::end(v5));
            cubeVertices.insert(cubeVertices.end(), std::begin(v4), std::end(v4));
            cubeVertices.insert(cubeVertices.end(), std::begin(v7), std::end(v7));

            cubeVertices.insert(cubeVertices.end(), std::begin(v5), std::end(v5));
            cubeVertices.insert(cubeVertices.end(), std::begin(v7), std::end(v7));
            cubeVertices.insert(cubeVertices.end(), std::begin(v6), std::end(v6));

            // Left face
            cubeVertices.insert(cubeVertices.end(), std::begin(v4), std::end(v4));
            cubeVertices.insert(cubeVertices.end(), std::begin(v0), std::end(v0));
            cubeVertices.insert(cubeVertices.end(), std::begin(v3), std::end(v3));

            cubeVertices.insert(cubeVertices.end(), std::begin(v4), std::end(v4));
            cubeVertices.insert(cubeVertices.end(), std::begin(v3), std::end(v3));
            cubeVertices.insert(cubeVertices.end(), std::begin(v7), std::end(v7));

            // Top face
            cubeVertices.insert(cubeVertices.end(), std::begin(v3), std::end(v3));
            cubeVertices.insert(cubeVertices.end(), std::begin(v2), std::end(v2));
            cubeVertices.insert(cubeVertices.end(), std::begin(v6), std::end(v6));

            cubeVertices.insert(cubeVertices.end(), std::begin(v3), std::end(v3));
            cubeVertices.insert(cubeVertices.end(), std::begin(v6), std::end(v6));
            cubeVertices.insert(cubeVertices.end(), std::begin(v7), std::end(v7));

            // Bottom face
            cubeVertices.insert(cubeVertices.end(), std::begin(v4), std::end(v4));
            cubeVertices.insert(cubeVertices.end(), std::begin(v5), std::end(v5));
            cubeVertices.insert(cubeVertices.end(), std::begin(v1), std::end(v1));

            cubeVertices.insert(cubeVertices.end(), std::begin(v4), std::end(v4));
            cubeVertices.insert(cubeVertices.end(), std::begin(v1), std::end(v1));
            cubeVertices.insert(cubeVertices.end(), std::begin(v0), std::end(v0));
			std::shared_ptr<Mesh> cube_mesh_ptr(new Mesh());
            cube_mesh_ptr->addGeometry(cubeVertices);
            cube_mesh_ptr->prepareVao();
			m_scene.add("Cube", cube_mesh_ptr);
		}

		void tick(float deltaTime)
		{
			m_controller.moveCamera(m_camera, deltaTime * 0.01f);
			m_controller.turnCamera(m_camera);
            m_camera.updateAspectRatio(static_cast<float>(Window::get().getWidth()) / Window::get().getHeight());
		}

		void draw()
		{
			m_renderer.clearBuffer();
			m_camera.draw(m_renderer.getShaderProgram());
			m_scene.draw(m_renderer.getShaderProgram(), m_camera.getViewMatrix());
		}

	private:
		Renderer   m_renderer;
		Controller m_controller;
		Camera     m_camera;
		Scene      m_scene;
	};
}