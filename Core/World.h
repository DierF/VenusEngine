#pragma once

#include "Editor/Window.h"
#include "Editor/Gui.h"
#include "Core/Scene.h"
#include "Core/SceneLight.h"
#include "Core/Camera.h"
#include "Core/Controller.h"
#include "Core/MouseBuffer.h"
#include "Core/WorldAxis.h"
#include "Render/Renderer.h"
#include "Render/Framebuffer.h"
#include "Render/Texture.h"
#include "Render/Renderbuffer.h"

namespace VenusEngine
{
	class World
	{
	public:
        World()
            // : m_renderer("../Render/Vec3.vert", "../Render/Vec3.frag"),
            : m_renderer("../Render/GeneralShader.vert", "../Render/GeneralShader.frag"),
              m_camera(Vec3(0.0f, 0.0f, 10.0f), Vec3(), 0.1f, 100.0f, 1200.0f / 900.0f, 60.0f),
              m_worldAxisEnabled(false)
		{
            // Diffuse intensity of the light source
            Vec3 diffuseIntensity(1.0f, 1.0f, 1.0f);
            // Specular intensity of the light source
            Vec3 specularIntensity(0.8f, 0.8f, 0.8f);
            // Direction of the light source in 3D space
            Vec3 direction(0.0f, 0.0f, -1.0f);
            std::shared_ptr<DirectionalLightSource> directional_light_ptr(new DirectionalLightSource(diffuseIntensity, specularIntensity, direction));
            if (!m_sceneLight.add("Light", directional_light_ptr))
            {
                std::cout << "Reached light source number limits" << std::endl;
            }

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

            m_depthbuffer.bind();
            m_depthbuffer.storage(Window::get().getWidth(), Window::get().getHeight());
            m_framebuffer.renderbuffer(GL_DEPTH_ATTACHMENT, m_depthbuffer.id());
            m_depthbuffer.unbind();

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
                m_controller.tickCamera(m_camera);
            }
            m_camera.updateAspectRatio(m_viewportSize.first / m_viewportSize.second);
		}

		void draw()
		{
            m_framebuffer.bind();

            // update active Mesh by selection
            if (m_viewportFocused && MouseBuffer::getPressedLeftButton() && !Gui::gizmoIsOver())
            {
                auto [clickedX, clickedY] = Input::GetMousePosition();
                // Window coordinate to viewport coordinate
                clickedX -= m_viewportPos.first;
                clickedY -= m_viewportPos.second;
                // Exclude tab bar size
                clickedY -= m_tabBarHeight;
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

            // update glviewport pos and size
            glViewport(0, m_tabBarHeight, m_viewportSize.first, m_viewportSize.second);
            
            m_texture.bind();
            m_texture.image2D(GL_RGBA, GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second), GL_RGBA, GL_UNSIGNED_BYTE);
            m_texture.unbind();

            m_IDTexture.bind();
            m_IDTexture.image2D(GL_R32I, GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second), GL_RED_INTEGER, GL_INT);
            m_IDTexture.unbind();

            m_depthbuffer.bind();
            m_depthbuffer.storage(GLsizei(m_viewportSize.first), GLsizei(m_viewportSize.second));
            m_depthbuffer.unbind();

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
            // Render World Axis
            if (m_worldAxisEnabled)
            {
                m_worldAxis.draw(m_renderer.getShaderProgram());
            }
            // Render Light
            m_sceneLight.draw(m_renderer.getShaderProgram());
            // Render Mesh
			m_scene.draw(m_renderer.getShaderProgram());

            if (m_scene.hasActiveMesh())
            {
                m_camera.getViewMatrix().transpose().toData(m_temp_view);
                m_camera.getProjectionMatrix().transpose().toData(m_temp_proj);
                m_scene.getActiveMesh()->getTransform().getMatrix().transpose().toData(m_temp_trans);
            }

            // Draw viewport window
            auto [viewportFocused, viewportSize, viewportPos, tabBarHeight] =
                Gui::viewportWindow(m_scene, m_texture.id(), m_temp_view, m_temp_proj, m_temp_trans);
            m_viewportFocused = viewportFocused;
            m_viewportSize    = viewportSize;
            m_viewportPos     = viewportPos;
            m_tabBarHeight    = tabBarHeight;

            if (m_scene.hasActiveMesh())
            {
                Transform& trans = m_scene.getActiveMesh()->getTransform();
                Mat4(m_temp_trans).transpose().decomposition(trans.m_position, trans.m_scale, trans.m_rotation);
            }

            m_framebuffer.unbind();
		}

	private:
		Renderer   m_renderer;
		Controller m_controller;
		Camera     m_camera;
        WorldAxis  m_worldAxis;
		Scene      m_scene;
        SceneLight m_sceneLight;

        Framebuffer  m_framebuffer;
        Texture      m_texture;
        Texture      m_IDTexture;
        Renderbuffer m_depthbuffer;

        bool                    m_viewportFocused;
        std::pair<float, float> m_viewportSize;
        std::pair<float, float> m_viewportPos;
        float                   m_tabBarHeight;

        bool m_worldAxisEnabled;

    public:
        // This is a temporary fix
        float m_temp_view[16];
        float m_temp_proj[16];
        float m_temp_trans[16];
	};
}