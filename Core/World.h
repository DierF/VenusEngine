#pragma once

#include "Editor/Window.h"
#include "Core/Scene.h"
#include "Core/SceneLight.h"
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
            // Render Light
            m_sceneLight.draw(m_renderer.getShaderProgram());
            // Render Mesh
			m_scene.draw(m_renderer.getShaderProgram());
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
        SceneLight m_sceneLight;

        Framebuffer             m_framebuffer;
        Texture                 m_texture;
        std::pair<float, float> m_viewportSize;
        bool                    m_viewportFocused;
	};
}