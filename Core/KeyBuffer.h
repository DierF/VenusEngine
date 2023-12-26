#pragma once

#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Editor/Window.h"

namespace VenusEngine
{
	class KeyBuffer
	{
	public:
		static bool getPressedKey(int key)
		{
			if (Window::get().m_keyWasPressed[key] == false)
			{
				return false;
			}
			Window::get().m_keyWasPressed[key] = false;
			return true;
		}
	};
}