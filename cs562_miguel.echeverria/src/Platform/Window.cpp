/**
* @file Window.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Implementation of the window functionality.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Window.h"
#include "Graphics/Systems/Renderer.h"
#include <GLFW/glfw3.h>


namespace cs460
{
	const int INITIAL_WINDOW_WIDTH = 1280;
	const int INITIAL_WINDOW_HEIGHT = 720;

	Window::Window()
		:	m_handle(nullptr),
			m_width(INITIAL_WINDOW_WIDTH),
			m_height(INITIAL_WINDOW_HEIGHT)
	{
	}

	Window::~Window()
	{
	}


	bool Window::create()
	{
		// Enable glfw error callbacks
		cs460::enable_glfw_debug_callbacks();

		// Initialize glfw
		if (!glfwInit())
		{
			std::cout << "ERROR: glfw initialization failed\n";
			return false;
		}

		// Set some window options before creating it
		set_window_hints();

		// Create a windowed mode window and its OpenGL context
		m_handle = glfwCreateWindow(m_width, m_height, "CS460 - Miguel Echeverria", nullptr, nullptr);
		if (!m_handle)
		{
			glfwTerminate();
			return false;
		}

		// Make the window's context current
		glfwMakeContextCurrent(m_handle);

		glfwSetWindowAspectRatio(m_handle, 16, 9);
		glfwSetFramebufferSizeCallback(m_handle, &on_framebuffer_resize);

		return true;
	}

	// Swaps the buffers, clears them, and polls for events
	void Window::update()
	{
		// TODO: Put this in another place
		if (glfwGetKey(m_handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
			glfwGetKey(m_handle, GLFW_KEY_Q) == GLFW_PRESS)
			set_window_should_close(1);

		glfwSwapBuffers(m_handle);
		glfwPollEvents();
	}

	void Window::close()
	{
		glfwDestroyWindow(m_handle);
		glfwTerminate();
	}


	void Window::set_swap_interval(int interval)
	{
		glfwSwapInterval(interval);
	}

	// Returns true if the window has been requested to be closed (for example the red x has been pressed)
	bool Window::get_window_should_close() const
	{
		return glfwWindowShouldClose(m_handle);
	}

	// Set the glfw flag that indicates that the window is requested to be closed
	void Window::set_window_should_close(int value)
	{
		glfwSetWindowShouldClose(m_handle, value);
	}

	GLFWwindow* Window::get_handle() const
	{
		return m_handle;
	}

	int Window::get_width() const
	{
		return m_width;
	}
	int Window::get_height() const
	{
		return m_height;
	}


	void Window::set_window_hints()
	{
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}


	// Callback for when the framebuffer is resized
	void on_framebuffer_resize(GLFWwindow* window, int newWidth, int newHeight)
	{
		Renderer::get_instance().set_viewport(0, 0, newWidth, newHeight);
	}
}