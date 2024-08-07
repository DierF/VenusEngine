#pragma once

#include <utility>

#include "Editor/Window.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Core/KeyBuffer.h"
#include "Core/MouseBuffer.h"

namespace VenusEngine
{
	class Controller
	{
	public:
		Controller()
			: m_lastMousePosition(Input::GetMousePosition())
		{
		}

        ~Controller() = default;

        void tickCamera(Camera& camera)
        {
            std::pair<float, float> currentPos = Input::GetMousePosition();
            std::pair<float, float> deltaPos = { currentPos.first  - m_lastMousePosition.first,
                                                 currentPos.second - m_lastMousePosition.second };
            m_lastMousePosition = currentPos;

            // tick camera if mouse right button being pressed
            if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
            {
                // move camera when shift + mouse right button
                if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || Input::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
                {
                    moveCamera(camera, deltaPos);
                }
                else
                {
                    turnCamera(camera, deltaPos);
                }
            }

            // Zoom camera
            float ScrollDelta = MouseBuffer::getScrolledDelta();
            if (ScrollDelta != 0.0f)
            {
                camera.zoom(-ScrollDelta);
            }
        }

        void moveCamera(Camera& camera, std::pair<float, float> deltaPos)
        {
            camera.moveRight(-deltaPos.first * 0.01f);
            camera.moveUp(deltaPos.second * 0.01f);
        }

        void turnCamera(Camera& camera, std::pair<float, float> deltaPos)
        {
            camera.rotateAroundHorizontally(-deltaPos.first);
            camera.rotateAroundVertically(-deltaPos.second);
        }

        bool shouldExitWorld()
        {
            bool escapePressed = KeyBuffer::getPressedKey(GLFW_KEY_ESCAPE);
            if (escapePressed)
            {
                Window::get().closeWindow();
            }
            return escapePressed;
        }

	private:
		std::pair<float, float> m_lastMousePosition;
	};
}