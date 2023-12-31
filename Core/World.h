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
            if (m_viewportFocused)
            {
                m_controller.moveCamera(m_camera, deltaTime * 0.01f);
                m_controller.turnCamera(m_camera);
            }
            m_camera.updateAspectRatio(static_cast<float>(Window::get().getWidth()) / Window::get().getHeight());
		}

		void draw()
		{
            m_framebuffer.bind();
            m_texture.bind();
            m_texture.image2D(GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second));
            m_texture.unbind();

            // Draw all meshes window
            auto const& [selected, selectedMeshName] = Gui::allMeshWindow(m_scene);
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
            auto [viewportFocused, viewportSize] = Gui::viewportWindow(m_texture.id());
            m_viewportFocused = viewportFocused;
            m_viewportSize    = viewportSize;

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
        bool                    m_viewportFocused;
	};
}