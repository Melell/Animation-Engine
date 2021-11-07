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
#include "GUI/EditorState.h"
#include "Composition/SceneNode.h"
#include <GLFW/glfw3.h>


namespace cs460
{
	const int INITIAL_WINDOW_WIDTH = 1280;
	const int INITIAL_WINDOW_HEIGHT = 720;

	Window::Window()
		:	m_handle(nullptr)
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
		m_handle = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "CS460 - Miguel Echeverria", nullptr, nullptr);
		if (!m_handle)
		{
			glfwTerminate();
			return false;
		}

		// Make the window's context current
		glfwMakeContextCurrent(m_handle);

		glfwSetWindowAspectRatio(m_handle, 16, 9);
		glfwSetFramebufferSizeCallback(m_handle, &on_framebuffer_resize);

		glfwSetDropCallback(m_handle, &on_files_dropped);

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

	int Window::get_window_width() const
	{
		int width, height;
		glfwGetWindowSize(m_handle, &width, &height);
		return width;
	}
	int Window::get_window_height() const
	{
		int width, height;
		glfwGetWindowSize(m_handle, &width, &height);
		return height;
	}
	void Window::get_window_dimensions(int* outWidth, int* outHeight) const
	{
		glfwGetWindowSize(m_handle, outWidth, outHeight);
	}

	int Window::get_framebuffer_width() const
	{
		int width, height;
		glfwGetFramebufferSize(m_handle, &width, &height);
		return width;
	}
	int Window::get_framebuffer_height() const
	{
		int width, height;
		glfwGetFramebufferSize(m_handle, &width, &height);
		return height;
	}
	void Window::get_framebuffer_dimensions(int* outWidth, int* outHeight) const
	{
		glfwGetFramebufferSize(m_handle, outWidth, outHeight);
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


	// Drag and drop callback (receives the paths to the files dropped)
	void on_files_dropped(GLFWwindow* window, int count, const char** paths)
	{
		Scene& scene = Scene::get_instance();
		EditorState& editorState = EditorState::get_main_editor_state();
		SceneNode* parent = editorState.m_selectedNode == nullptr ? scene.get_root() : editorState.m_selectedNode;
		SceneNode* lastCreatedNode = nullptr;

		// For every given path
		for (int i = 0; i < count; i++)
		{
			fs::path path(paths[i]);

			// If its a directory, iterate over its contents
			if (fs::is_directory(path))
			{
				for (const fs::directory_entry& currDir : fs::recursive_directory_iterator(path))
				{
					// GLTF file found, add it as child of currently selected node
					if (currDir.path().extension().generic_string() == ".gltf")
						lastCreatedNode = parent->create_child_with_model(currDir.path().filename().generic_string(), currDir.path());
				}
			}
			// Otherwise, if we found a gltf file, add it as a child of the currently selected node
			else if (path.extension().generic_string() == ".gltf")
				lastCreatedNode = parent->create_child_with_model(path.filename().generic_string(), path);
		}


		// Set the selected node as the last one that was created
		if (lastCreatedNode != nullptr)
			editorState.m_selectedNode = lastCreatedNode;
	}
}