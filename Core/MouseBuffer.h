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

		static float getScrolledDelta()
		{
			float delta = Window::get().m_mouseScrollDelta;
			if (delta != 0.0f)
			{
				Window::get().m_mouseScrollDelta = 0.0f;
			}
			return delta;
		}
	};
}