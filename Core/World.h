#pragma once

#include "Editor/Window.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Core/Controller.h"
#include "Render/Renderer.h"
#include "Render/Framebuffer.h"
#include "Render/Texture.h"

namespace VenusEngine
{
	class World
	{
	public:
		World()
			: m_camera(Vec3(0.0f, 0.0f, 15.0f), 0.0f, 0.0f, 0.1f, 1000.0f, 1200.0f / 900.0f, 45.0f)
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
            float v0[] = { -side / 2, -side / 2, -side / 2, 1.0f, 0.0f, 0.0f }; // Red
            float v1[] = {  side / 2, -side / 2, -side / 2, 1.0f, 1.0f, 0.0f }; // Yellow
            float v2[] = {  side / 2,  side / 2, -side / 2, 0.0f, 1.0f, 0.0f }; // Green
            float v3[] = { -side / 2,  side / 2, -side / 2, 0.0f, 0.0f, 1.0f }; // Blue
            float v4[] = { -side / 2, -side / 2,  side / 2, 1.0f, 1.0f, 0.0f }; // Yellow
            float v5[] = {  side / 2, -side / 2,  side / 2, 0.0f, 1.0f, 0.0f }; // Green
            float v6[] = {  side / 2,  side / 2,  side / 2, 0.0f, 0.0f, 1.0f }; // Blue
            float v7[] = { -side / 2,  side / 2,  side / 2, 1.0f, 0.0f, 0.0f }; // Red

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
            cube_mesh_ptr->getTransform().m_position.x += 7.0f;
			m_scene.add("Cube", cube_mesh_ptr);

            m_framebuffer.bind();
            m_texture.bind();
            m_texture.image2D(Window::get().getWidth(), Window::get().getHeight());
            m_texture.filter();
            m_texture.unbind();
            m_framebuffer.texture(m_texture.id());
		}

		void tick(float deltaTime)
		{
            // Check for quit
            if (m_controller.shouldExitWorld())
            {
                return;
            }
            // Tick camera
			m_controller.moveCamera(m_camera, deltaTime * 0.01f);
			m_controller.turnCamera(m_camera);
            m_camera.updateAspectRatio(static_cast<float>(Window::get().getWidth()) / Window::get().getHeight());
            // Update active mesh
            auto pos = m_controller.updateActiveMesh();
            if (pos != std::pair<float, float>())
            {
                std::cout << "[ " << pos.first << ", " << pos.second << " ]\n";
            }
		}

		void draw()
		{
            m_framebuffer.bind();
            m_texture.bind();
            m_texture.image2D(GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second));
            m_texture.unbind();

            // Draw all meshes window
            auto const& [selected, selectedMeshName] = Gui::allMeshWindow(m_scene.activeMeshName(), m_scene.allMeshNames());
            if (selected)
            {
                m_scene.setActiveMesh(selectedMeshName);
            }
            // Draw active mesh window
            Gui::activeMeshWindow(m_scene);
            // Clear buffer bit
			m_renderer.clearBuffer();
            // Render camera
			m_camera.draw(m_renderer.getShaderProgram());
            // Render Mesh
			m_scene.draw(m_renderer.getShaderProgram(), m_camera.getViewMatrix());
            // Draw viewport window
            m_viewportSize = Gui::viewportWindow(m_texture.id());

            m_framebuffer.unbind();
		}

	private:
		Renderer   m_renderer;
		Controller m_controller;
		Camera     m_camera;
		Scene      m_scene;

        Framebuffer             m_framebuffer;
        Texture                 m_texture;
        std::pair<float, float> m_viewportSize;
	};
}