#pragma once

#include "Editor/Window.h"
#include "Core/Scene.h"

namespace VenusEngine
{
	class Application
	{
	public:
		Application() = default;

		~Application() = default;

		void run()
		{
			while (!m_window.shouldClose())
			{
				m_scene.draw();
				m_window.update();
			}
		}

	private:
		Window m_window;
		Scene  m_scene;
	};
}