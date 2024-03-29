#pragma once

#include <cassert>
#include <stdexcept>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace VenusEngine
{
	class Window
	{
		friend class KeyBuffer;
		friend class MouseBuffer;

	public:
		Window()
		{
			s_instance = this;

			if (!glfwInit())
			{
				throw std::runtime_error("glfwInit failed");
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

#ifdef __APPLE__
			// Necessary on MacOS!
			// Set forward compatibility bit
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_window = glfwCreateWindow(m_width, m_height, "ParallelParallel", nullptr, nullptr);
			assert(m_window);

			glfwMakeContextCurrent(m_window);

			glfwSwapInterval(1);

			glfwSetErrorCallback(glfw_error_callback);
			glfwSetKeyCallback(m_window, keyCallback);
			glfwSetMouseButtonCallback(m_window, mouseCallback);
			glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				throw std::runtime_error("failed to initialize GLAD");
			}

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_PROGRAM_POINT_SIZE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			std::memset(m_keyWasPressed, 0, sizeof(m_keyWasPressed));

			m_mouseLeftButtonPressed = false;
		}

		~Window()
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		void update()
		{
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		void closeWindow()
		{
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		bool shouldClose() const
		{
			return glfwWindowShouldClose(m_window);
		}

		GLFWwindow* nativeWindow()
		{
			return m_window;
		}

		int getWidth() const
		{
			return m_width;
		}

		int getHeight() const
		{
			return m_height;
		}

	private:
		static void glfw_error_callback(int error, const char* description)
		{
			fprintf(stderr, "GLFW Error %d: %s\n", error, description);
		}

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				s_instance->m_keyWasPressed[key] = true;
			}
		}

		static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				double xpos, ypos;
				glfwGetCursorPos(window, &xpos, &ypos);
				s_instance->m_mouseLeftButtonPressed = true;
				s_instance->m_mousePos.first = float(xpos);
				s_instance->m_mousePos.second = float(ypos);
			}
		}

		static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			s_instance->m_width  = width;
			s_instance->m_height = height;
		}

	private:
		GLFWwindow* m_window = nullptr;
		int         m_width  = 1600;
		int         m_height = 900;

		bool m_keyWasPressed[GLFW_KEY_LAST + 1];

		bool m_mouseLeftButtonPressed;
		std::pair<float, float> m_mousePos;

	public:
		static Window& get()
		{
			return *s_instance;
		}

	private:
		static Window* s_instance;
	};
}