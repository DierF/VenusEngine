#pragma once

#include "Editor/Window.h"

namespace VenusEngine
{
	class MouseBuffer
	{
	public:
		static bool getPressedLeftButton()
		{
			if (Window::get().m_mouseLeftButtonPressed == false)
			{
				return false;
			}
			Window::get().m_mouseLeftButtonPressed = false;
			return true;
		}

		static std::pair<float, float> getPressedLeftButtonPos()
		{
			return Window::get().m_mousePos;
		}
	};
}