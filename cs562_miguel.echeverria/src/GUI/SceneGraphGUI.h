/**
* @file SceneGraphGUI.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief In charge of the scene graph gui logic.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class SceneNode;


	class SceneGraphGUI
	{
	public:

		friend class Editor;	// Only Editor can create instances of SceneGraphGUI

		void update();			// Logic for the scene graph window

	private:

		void display_node_info(SceneNode* node);	// Helper function to display the scene graph recursively as a tree
		void node_popup();							// Displays a popup with special options like creating a new node (called when a node is right clicked)
		void new_node_name_popup();					// Displays a popup modal for setting the name of the new node

		void delete_node();

		bool m_newnode_popup_active = false;
		bool m_newname_popup_active = false;

		SceneGraphGUI();
	};
}