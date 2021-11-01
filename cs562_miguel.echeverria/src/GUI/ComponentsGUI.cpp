/**
* @file ComponentsGUI.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief In charge of the components editor window.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ComponentsGUI.h"
#include "EditorState.h"
#include "Composition/SceneNode.h"


namespace cs460
{
	ComponentsGUI::ComponentsGUI()
	{
	}

	void ComponentsGUI::update()
	{
		if (!ImGui::Begin("Component Editor"))
		{
			ImGui::End();
			return;
		}

		EditorState& state = EditorState::get_main_editor_state();

		// Only draw the gui if an object is selected
		if (state.m_selectedNode)
			state.m_selectedNode->on_gui();

		ImGui::End();
	}
}