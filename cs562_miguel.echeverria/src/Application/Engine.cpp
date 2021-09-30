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
#include "GUI/Editor.h"
#include "Composition/Scene.h"


namespace cs460
{
	// Initialization of the engine and all its systems
	bool Engine::initialize()
	{
		// IMPORTANT!! Initialize the renderer first, as it initializes glfw, which might be needed for other systems like input
		if (!Renderer::get_instance().initialize())
			return false;

		// Initialize imgui and set up the context for gui drawing
		if (!Editor::get_instance().initialize())
			return false;

		// Initialize the scene graph (create the root node)
		if (!Scene::get_instance().initialize())
			return false;

		return true;
	}


	// Update of the engine and all its necessary systems
	void Engine::update()
	{
		Renderer& renderer = Renderer::get_instance();
		Editor& editor = Editor::get_instance();
		
		// Loop until the user closes the window
		while (!renderer.get_window().get_window_should_close())
		{
			// Do all the gui logic
			editor.update();

			// Render the scene
			renderer.render();

			// Render the gui
			editor.render();

			// Swap buffers, clear the back buffer and poll for events
			renderer.get_window().update();

			// Clear the frame buffer for the next frame
			renderer.clear_fb();
		}
	}


	// Close the engine by releasing any resources in use
	void Engine::close()
	{
		Scene::get_instance().close();					// Release the memory of all the scene nodes
		Editor::get_instance().close();					// Terminate imgui
		Renderer::get_instance().close();				// Buffer cleanup
		Renderer::get_instance().get_window().close();	// Terminate glfw
	}
}