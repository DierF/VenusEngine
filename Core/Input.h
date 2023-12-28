#pragma once

#include "Editor/Window.h"

namespace VenusEngine
{
	class Input
	{
	public:
		static bool IsKeyPressed(int key)
		{
			int state = glfwGetKey(Window::get().nativeWindow(), key);
			return state == GLFW_PRESS;
		}

		static bool IsMousePressed(int mouse)
		{
			int state = glfwGetMouseButton(Window::get().nativeWindow(), mouse);
			return state == GLFW_PRESS;
		}

		static std::pair<float, float> GetMousePosition()
		{
			double x, y;
			glfwGetCursorPos(Window::get().nativeWindow(), &x, &y);
			return { float(x), float(y) };
		}
	};
}