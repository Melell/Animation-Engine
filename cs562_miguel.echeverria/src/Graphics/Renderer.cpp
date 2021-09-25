/**
* @file Renderer.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Implementation of the class that is in charge of rendering the scene.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Renderer.h"
#include <GL/glew.h>


namespace cs460
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	Renderer& Renderer::get_instance()
	{
		static Renderer instance;
		return instance;
	}


	bool Renderer::initialize()
	{
		// Initialize glfw, create the window and associate an opengl context to it
		if (!m_window.create())
			return false;

		// Load all the opengl functions using glew
		GLenum glewStatus = glewInit();
		if (glewStatus != GLEW_OK)
		{
			std::cout << "ERROR: GLEW initialization failed\n";
			return false;
		}

		// Enable opengl error callbacks and call some other opengl configuration functions (for example setting up vsync or depth testing)
		cs460::enable_gl_debug_callbacks();
		set_gl_properties();

		return true;
	}

	void Renderer::render()
	{
	}

	void Renderer::close()
	{
		// Release all buffers???

		
	}

	Window& Renderer::get_window()
	{
		return m_window;
	}


	void Renderer::set_gl_properties()
	{
		m_window.set_swap_interval(1);				// Enable v-sync
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);

		//std::cout << glGetString(GL_VERSION) << std::endl;
	}
}