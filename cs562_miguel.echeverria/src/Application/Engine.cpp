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
#include "Graphics/Systems/Renderer.h"
#include "GUI/Editor.h"
#include "Composition/Scene.h"
#include "Resources/ResourceManager.h"
#include "Platform/FrameRateController.h"
#include "Animation/Animator.h"
#include "Animation/PiecewiseCurveMgr.h"
#include "Platform/InputMgr.h"
#include "GUI/MainMenuBarGUI.h"		// TODO: Remove this in the future


namespace cs460
{
	// Initialization of the engine and all its systems
	bool Engine::initialize()
	{
		// IMPORTANT!! Initialize the renderer first, as it initializes glfw, which might be needed for other systems like input
		Renderer& renderer = Renderer::get_instance();
		if (!renderer.initialize())
			return false;

		// Initialize the input system (set key, mouse etc callbacks)
		InputMgr& inputMgr = InputMgr::get_instance();
		if (!inputMgr.initialize())
			return false;

		// Initialize the animation system
		Animator& animator = Animator::get_instance();
		if (!animator.initialize())
			return false;

		// Initialize imgui and set up the context for gui drawing
		Editor& editor = Editor::get_instance();
		if (!editor.initialize())
			return false;

		// Initialize the scene graph (create the root node)
		Scene& scene = Scene::get_instance();
		if (!scene.initialize())
			return false;

		// Load all the necessary resources beforehand (mainly shaders and skyboxes), and set the initial skybox
		ResourceManager& resourceManager = ResourceManager::get_instance();
		resourceManager.load_resources();
		renderer.change_skybox("WaterAndMountains");

		return true;
	}


	// Update of the engine and all its necessary systems
	void Engine::update()
	{
		Renderer& renderer = Renderer::get_instance();
		Animator& animator = Animator::get_instance();
		Editor& editor = Editor::get_instance();
		Scene& scene = Scene::get_instance();
		FrameRateController& frc = FrameRateController::get_instance();
		PiecewiseCurveMgr& curveMgr = PiecewiseCurveMgr::get_instance();
		InputMgr& inputMgr = InputMgr::get_instance();

		
		// Loop until the user closes the window
		while (!renderer.get_window().get_window_should_close())
		{
			// Update the editor camera (the way the camera is organized will change)
			scene.get_camera().update();

			// Update all the model to local and model to world matrices
			scene.update();

			// Do all the gui logic as well as the gizmos
			editor.update();

			// Update all the piecewise curves
			curveMgr.update();

			// Update the animations and joint matrices
			animator.update();

			// Render the scene
			renderer.render();

			// Render the gui
			editor.render();

			// Update the input system (needs to be called just before polling for events)
			inputMgr.update();

			// Swap buffers, clear the back buffer and poll for events
			renderer.get_window().update();

			// Clear the frame buffer for the next frame
			renderer.clear_fb();

			MainMenuBarGUI::get_main_menu_bar_gui().load_scene();

			// End the measurement and store a new dt
			frc.end_frame();
		}
	}


	// Close the engine by releasing any resources in use
	void Engine::close()
	{
		Scene::get_instance().close();					// Release the memory of all the scene nodes
		Editor::get_instance().close();					// Terminate imgui
		Animator::get_instance().close();				// Terminate the animation system
		InputMgr::get_instance().close();
		Renderer::get_instance().close();
		Renderer::get_instance().get_window().close();	// Terminate glfw
	}
}