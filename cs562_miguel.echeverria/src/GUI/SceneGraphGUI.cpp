/**
* @file SceneGraphGUI.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the implementation of the class in charge of the scene graph gui.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "SceneGraphGUI.h"
#include "Composition/Scene.h"
#include "Composition/SceneNode.h"
#include "Editor.h"
#include <imgui/imgui.h>


namespace cs460
{
	SceneGraphGUI::SceneGraphGUI()
	{
	}

	// Logic for the scene graph window
	void SceneGraphGUI::update()
	{
		if (!ImGui::Begin("Scene Graph"))
		{
			ImGui::End();
			return;
		}

		SceneNode* tree = Scene::get_instance().get_root();
		display_node_info(tree);

		if (m_name_popup_active)
			ImGui::OpenPopup("Set Name Popup");

		new_node_name_popup();

		ImGui::End();
	}


	// Helper function to display the scene graph recursively as a tree
	void SceneGraphGUI::display_node_info(SceneNode* node)
	{
		if (node == nullptr)
			return;

		EditorState& state = Editor::get_instance().get_state();

		// Base flags: Open on arrow or double click, and let the hit box span the whole width of the window
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

		// If the current node is the selected one, mark as selected
		if (node == state.m_selectedNode)
			nodeFlags |= ImGuiTreeNodeFlags_Selected;

		// If it has no more children, mark it as leaf (no arrow)
		if (node->get_children().empty())
			nodeFlags |= ImGuiTreeNodeFlags_Leaf;

		bool nodeOpen = ImGui::TreeNodeEx(node->m_name.c_str(), nodeFlags);
		bool itemLeftClicked = ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left);
		bool itemRightClicked = ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Right);

		// If the node is open
		if (nodeOpen)
		{
			// Recursively display all its children
			const std::vector<SceneNode*>& children = node->get_children();
			for (int i = 0; i < children.size(); ++i)
				display_node_info(children[i]);

			ImGui::TreePop();
		}
		
		// Create the node popup (only visible when right clicking a node)
		if (node == state.m_selectedNode)
			node_popup();

		// Left click, select it
		if (itemLeftClicked)
			state.m_selectedNode = node;

		// Right click, open context menu
		if (itemRightClicked)
		{
			state.m_selectedNode = node;
			ImGui::OpenPopup("Right Click Popup");
		}
	}


	// Displays a popup with special options like creating a new node (called when a node is right clicked)
	void SceneGraphGUI::node_popup()
	{
		EditorState& state = Editor::get_instance().get_state();
		bool name_popup_active = false;

		if (ImGui::BeginPopup("Right Click Popup"))
		{
			if (ImGui::Selectable("Add New Node"))
				m_name_popup_active = true;

			ImGui::EndPopup();
		}
	}


	// Displays a popup modal for setting the name of the new node
	void SceneGraphGUI::new_node_name_popup()
	{
		EditorState& state = Editor::get_instance().get_state();

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Set Name Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Please enter the new node's name.\n");
			ImGui::Separator();

			ImGui::InputText("Name", state.m_textBuffer, IM_ARRAYSIZE(state.m_textBuffer));

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				state.m_selectedNode->create_child(state.m_textBuffer);
				state.m_selectedNode = state.m_selectedNode->get_children().back();
				m_name_popup_active = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				m_name_popup_active = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}