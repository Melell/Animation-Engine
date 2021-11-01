/**
* @file Editor.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the Editor singleton class that is in charge of the main editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/


#pragma once

#include "EditorState.h"
#include "MainMenuBarGUI.h"
#include "SceneGraphGUI.h"
#include "ComponentsGUI.h"
#include "RendererConfigGUI.h"
#include "CameraControlsGUI.h"
#include "Gizmos.h"


namespace cs460
{
	class SceneNode;


	class Editor
	{
	public:
	
		~Editor();
	
		static Editor& get_instance();
	
		bool initialize();		// Setup imgui for rendering
		void update();			// Start a new imgui frame and do all the gui logic
		void render();			// Render the gui
		void close();			// Terminate imgui

		EditorState& get_state();

		// TODO: Remove this in the future
		MainMenuBarGUI& get_main_menu_bar();
	
	private:
	
		EditorState m_state;
		MainMenuBarGUI m_mainMenuBar;
		SceneGraphGUI m_sceneGraphGui;
		ComponentsGUI m_componentEditor;
		RendererConfigGUI m_rendererConfigGui;
		CameraControlsGUI m_cameraControlsGui;
		Gizmos m_gizmos;

		Editor();
		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;
	};
}