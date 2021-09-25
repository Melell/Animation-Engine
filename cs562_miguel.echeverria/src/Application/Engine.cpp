/**
* @file Engine.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the implementation of the Engine singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Engine.h"
#include "Graphics/Renderer.h"


namespace cs460
{
	// Initialization of the engine and all its systems
	bool Engine::initialize()
	{
		// Initialize the renderer
		if (!Renderer::get_instance().initialize())
			return false;

		return true;
	}


	// Update of the engine and all its necessary systems
	void Engine::update()
	{
		Renderer& renderer = Renderer::get_instance();
		
		// Loop until the user closes the window
		while (!renderer.get_window().get_window_should_close())
		{
			// Render the scene
			renderer.render();

			// Swap buffers, clear the back buffer and poll for events
			renderer.get_window().update();
		}
	}


	// Close the engine by releasing any resources in use
	void Engine::close()
	{
		Renderer::get_instance().close();
		Renderer::get_instance().get_window().close();	// Terminate glfw
	}
}