#pragma once

#include "Editor/Window.h"
#include "Core/World.h"

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
				m_world.draw();
				m_window.update();
			}
		}

	private:
		Window m_window;
		World  m_world;
	};
}