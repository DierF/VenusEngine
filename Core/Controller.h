#pragma once

#include <utility>

#include "Core/Input.h"
#include "Core/Camera.h"

namespace VenusEngine
{
	class Controller
	{
	public:
		Controller()
			: m_lastMousePosition({ 0.0f, 0.0f }),
			  m_mouseRightButtonPressed(false)
		{
		}

        ~Controller()
        {
        }

        void moveCamera(Camera& camera, float distance)
        {
            if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || Input::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT))
            {
                distance *= 2.0f;
            }
            if (Input::IsKeyPressed(GLFW_KEY_W))
            {
                camera.moveFront(distance);
            }
            if (Input::IsKeyPressed(GLFW_KEY_A))
            {
                camera.moveRight(-distance);
            }
            if (Input::IsKeyPressed(GLFW_KEY_S))
            {
                camera.moveFront(-distance);
            }
            if (Input::IsKeyPressed(GLFW_KEY_D))
            {
                camera.moveRight(distance);
            }
        }

        void turnCamera(Camera& camera)
        {
            float deltaYaw   = 0.0f;
            float deltaPitch = 0.0f;
            if (Input::IsMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
            {
                auto pos = Input::GetMousePosition();
                if (m_mouseRightButtonPressed)
                {
                    deltaYaw   = (pos.first  - m_lastMousePosition.first)  * 0.05f;
                    deltaPitch = (pos.second - m_lastMousePosition.second) * 0.05f;
                }
                m_mouseRightButtonPressed = true;
                m_lastMousePosition = pos;
            }
            else
            {
                m_mouseRightButtonPressed = false;
            }
            camera.moveYawAndPitch(deltaYaw, deltaPitch);
        }

	private:
		std::pair<float, float> m_lastMousePosition;
		bool                    m_mouseRightButtonPressed;
	};
}