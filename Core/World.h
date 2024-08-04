#pragma once

#include "Editor/Window.h"
#include "Core/Scene.h"
#include "Core/SceneLight.h"
#include "Core/Camera.h"
#include "Core/Controller.h"
#include "Core/MouseBuffer.h"
#include "Render/Renderer.h"
#include "Render/Framebuffer.h"
#include "Render/Texture.h"

namespace VenusEngine
{
	class World
	{
	public:
		World()
			// : m_renderer("../Render/Vec3.vert", "../Render/Vec3.frag"),
			: m_renderer("../Render/GeneralShader.vert", "../Render/Vec3.frag"),
              m_camera(Vec3(0.0f, 0.0f, 15.0f), 0.0f, 0.0f, 0.1f, 1000.0f, 1200.0f / 900.0f, 45.0f)
		{
            // Diffuse intensity of the light source
            Vec3 diffuseIntensity(1.0f, 1.0f, 1.0f);
            // Specular intensity of the light source
            Vec3 specularIntensity(0.8f, 0.8f, 0.8f);
            // Direction of the light source in 3D space
            Vec3 direction(0.0f, 0.0f, -1.0f);
            std::shared_ptr<DirectionalLightSource> directional_light_ptr(new DirectionalLightSource(diffuseIntensity, specularIntensity, direction));
            m_sceneLight.add("Light", directional_light_ptr);

            m_framebuffer.bind();

            m_texture.bind();
            m_texture.image2D(GL_RGBA, Window::get().getWidth(), Window::get().getHeight(), GL_RGBA, GL_UNSIGNED_BYTE);
            m_texture.filter(GL_LINEAR);
            m_texture.unbind();
            m_framebuffer.texture2D(GL_COLOR_ATTACHMENT0, m_texture.id());

            m_IDTexture.bind();
            m_IDTexture.image2D(GL_R32I, Window::get().getWidth(), Window::get().getHeight(), GL_RED_INTEGER, GL_INT);
            m_IDTexture.filter(GL_NEAREST);
            m_IDTexture.unbind();
            m_framebuffer.texture2D(GL_COLOR_ATTACHMENT1, m_IDTexture.id());

            GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            m_renderer.drawBuffers(2, drawBuffers);

            glReadBuffer(GL_COLOR_ATTACHMENT1);

            m_framebuffer.unbind();
		}

		void tick(float deltaTime)
		{
            // Check for quit
            if (m_controller.shouldExitWorld())
            {
                return;
            }
            // Tick viewport objects
            if (m_viewportFocused)
            {
                // tick camera
                m_controller.moveCamera(m_camera, deltaTime * 0.01f);
                m_controller.turnCamera(m_camera);
            }
            m_camera.updateAspectRatio(static_cast<float>(Window::get().getWidth()) / Window::get().getHeight());
		}

		void draw()
		{
            m_framebuffer.bind();

            // update active Mesh by selection
            if (m_viewportFocused && MouseBuffer::getPressedLeftButton())
            {
                auto [clickedX, clickedY] = MouseBuffer::getPressedLeftButtonPos();
                // Window coordinate to viewport coordinate
                clickedX -= m_viewportPos.first;
                clickedY -= m_viewportPos.second;
                // Exclude tarBar size
                clickedY += m_tabBarHeight;
                // Invert y coordinate(viewport coordinate to OpenGL coordinate)
                int viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                clickedY = viewport[3] - clickedY;
                int id = -1;
                glReadPixels(clickedX, clickedY, 1, 1, GL_RED_INTEGER, GL_INT, &id);
                if (id != -1)
                {
                    m_scene.setActiveMeshByID(id);
                }
            }
            
            m_texture.bind();
            m_texture.image2D(GL_RGBA, GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second), GL_RGBA, GL_UNSIGNED_BYTE);
            m_texture.unbind();

            m_IDTexture.bind();
            m_IDTexture.image2D(GL_R32I, GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second), GL_RED_INTEGER, GL_INT);
            m_IDTexture.unbind();

            // Draw all meshes window
            auto const& selectedMeshName = Gui::allMeshWindow(m_scene);
            if (!selectedMeshName.empty())
            {
                m_scene.setActiveMesh(selectedMeshName);
            }

            // Draw active mesh window
            Gui::activeMeshWindow(m_scene);
            // Clear buffer bit
			m_renderer.clearBuffer();
            // Render camera
			m_camera.draw(m_renderer.getShaderProgram());
            // Render Light
            m_sceneLight.draw(m_renderer.getShaderProgram());
            // Render Mesh
			m_scene.draw(m_renderer.getShaderProgram());
            // Draw viewport window
            auto [viewportFocused, viewportSize, viewportPos, tabBarHeight] = Gui::viewportWindow(m_texture.id());
            m_viewportFocused = viewportFocused;
            m_viewportSize    = viewportSize;
            m_viewportPos     = viewportPos;
            m_tabBarHeight    = tabBarHeight;

            m_framebuffer.unbind();
		}

	private:
		Renderer   m_renderer;
		Controller m_controller;
		Camera     m_camera;
		Scene      m_scene;
        SceneLight m_sceneLight;

        Framebuffer             m_framebuffer;
        Texture                 m_texture;
        Texture                 m_IDTexture;

        bool                    m_viewportFocused;
        std::pair<float, float> m_viewportSize;
        std::pair<float, float> m_viewportPos;
        float                   m_tabBarHeight;
	};
}