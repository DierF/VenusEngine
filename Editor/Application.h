#pragma once

#include "Editor/Window.h"
#include "Core/World.h"
#include "Core/Time.h"

namespace VenusEngine
{
	class Application
	{
	public:
		Application() = default;

		~Application() = default;

		void run()
		{
			m_timer.reset();
			while (!m_window.shouldClose())
			{
				float deltaTime = m_timer.elapsedMilliseconds();
				m_timer.reset();
				m_world.tick(deltaTime);
				m_world.draw();
				m_window.update();
			}
		}

	private:
		Timer m_timer;
		Window m_window;
		World  m_world;
	};
}