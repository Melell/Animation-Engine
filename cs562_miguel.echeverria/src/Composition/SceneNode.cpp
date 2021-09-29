/**
* @file SceneNode.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Implementation of the class that represents a node in the scene
*		 graph (like a gameobject), already containing transform information.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "SceneNode.h"
#include "Components/IComponent.h"
#include "Components/ModelInstance.h"
#include <imgui/imgui.h>


namespace cs460
{
	SceneNode::SceneNode(const std::string& name)
		:	m_name(name),
			m_parent(nullptr)
	{

	}
	SceneNode::~SceneNode()
	{

	}

	// Create a new node and add it as this node's child
	SceneNode* SceneNode::create_child(const std::string& name)
	{
		SceneNode* newNode = new SceneNode(name);
		newNode->m_parent = this;
		m_children.push_back(newNode);
		return newNode;
	}

	// Show the components gui
	void SceneNode::on_gui()
	{
		ImGui::Text("Name: %s", m_name.c_str());
		ImGui::Separator();
		ImGui::NewLine();

		show_transforms_gui();

		for (int i = 0; i < m_components.size(); ++i)
			m_components[i]->show_gui();

		ImGui::NewLine();
		ImGui::Separator();

		if (ImGui::Button("New Component"))
			ImGui::OpenPopup("New Comp Popup");

		if (ImGui::BeginPopup("New Comp Popup"))
		{
			// Horrible hardcode, this will change
			if (ImGui::Selectable("ModelInstance"))
			{
				add_component<ModelInstance>();
			}

			ImGui::EndPopup();
		}
	}

	SceneNode* SceneNode::get_parent() const
	{
		return m_parent;
	}

	const std::vector<SceneNode*>& SceneNode::get_children() const
	{
		return m_children;
	}

	const std::vector<IComponent*>& SceneNode::get_components() const
	{
		return m_components;
	}


	void SceneNode::show_transforms_gui()
	{
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Local");
			ImGui::DragFloat3("Position", glm::value_ptr(m_localTr.m_position));
			ImGui::DragFloat3("Rotation", glm::value_ptr(m_localTr.m_orientation));
			ImGui::DragFloat3("Scale", glm::value_ptr(m_localTr.m_scale));

			//ImGui::NewLine();
			ImGui::Separator();
			//ImGui::NewLine();

			ImGui::Text("World");
			ImGui::DragFloat3("Position", glm::value_ptr(m_worldTr.m_position));
			ImGui::DragFloat3("Rotation", glm::value_ptr(m_worldTr.m_orientation));
			ImGui::DragFloat3("Scale", glm::value_ptr(m_worldTr.m_scale));
		}
	}
}