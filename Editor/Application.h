#pragma once

#include "Editor/Window.h"
#include "Editor/Gui.h"
#include "Core/World.h"
#include "Core/Time.h"

namespace VenusEngine
{
	class Application
	{
	public:
		Application()
		{
			Gui::init(m_window.nativeWindow());
		}

		~Application()
		{
			Gui::destroy();
		}

		void run()
		{
			m_timer.reset();
			while (!m_window.shouldClose())
			{
				float deltaTime = m_timer.elapsedMilliseconds();
				m_timer.reset();
				Gui::newFrame();
				Gui::beginDockspace();
				m_world.tick(deltaTime);
				m_world.draw();
				Gui::endDockspace();
				Gui::draw();
				m_window.update();
			}
		}

	private:
		Timer  m_timer;
		Window m_window;
		World  m_world;
	};
}