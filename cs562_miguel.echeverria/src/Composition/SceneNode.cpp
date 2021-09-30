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
#include "Graphics/Model.h"
#include "Components/MeshRenderable.h"
#include <imgui/imgui.h>
#include <gltf/tiny_gltf.h>


namespace cs460
{
	SceneNode::SceneNode(const std::string& name)
		:	m_name(name),
			m_parent(nullptr)
	{

	}
	SceneNode::~SceneNode()
	{
		// Clear in case there are still components/children
		clear();
	}

	// Free all the components of this node
	void SceneNode::delete_all_components()
	{
		for (int i = 0; i < m_components.size(); ++i)
		{
			delete m_components[i];
			m_components[i] = nullptr;
		}
		m_components.clear();
	}

	// Free all the children of this node
	void SceneNode::delete_all_children()
	{
		for (int i = 0; i < m_children.size(); ++i)
			m_children[i]->clear();

		m_children.clear();
	}

	// Free all the components and children of this node
	void SceneNode::clear()
	{
		delete_all_components();
		delete_all_children();
	}

	// Create a new node and add it as this node's child (from name)
	SceneNode* SceneNode::create_child(const std::string& name)
	{
		SceneNode* newNode = new SceneNode(name);
		newNode->m_parent = this;
		m_children.push_back(newNode);
		return newNode;
	}

	// Generates all the data for this scenenode from a gltf node
	void SceneNode::from_gltf_node(const tinygltf::Model& model, const tinygltf::Node& node, Model const* sourceRsrc, SceneNode const* modelRootNode)
	{
		m_name = node.name;
		set_localtr_from_gltf_node(node);
		
		// If this node uses a mesh, save its index in the model's vector of meshes (by generating a mesh renderable for drawing)
		if (node.mesh >= 0 && node.mesh < model.meshes.size())
		{
			MeshRenderable* comp = add_component<MeshRenderable>();
			comp->set_model_src(sourceRsrc);
			comp->set_mesh_idx(node.mesh);
			comp->set_model_root_node(modelRootNode);
		}

		// Create the child nodes
		for (int i = 0; i < node.children.size(); ++i)
		{
			int childIdx = node.children[i];
			const tinygltf::Node& childNode = model.nodes[childIdx];
			SceneNode* child = create_child(childNode.name);
			child->from_gltf_node(model, childNode, sourceRsrc, modelRootNode);
		}
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
			// Horrible hardcode, this will change in the future
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

	void SceneNode::set_localtr_from_gltf_node(const tinygltf::Node& node)
	{
		bool locationSet = false;
		bool scaleSet = false;
		bool orientationSet = false;

		// Set the local position, scale and orientation from the gltf node data if it was provided
		if (node.translation.size())
		{
			m_localTr.m_position.x = node.translation[0];
			m_localTr.m_position.y = node.translation[1];
			m_localTr.m_position.z = node.translation[2];
			locationSet = true;
		}
		if (node.scale.size())
		{
			m_localTr.m_scale.x = node.scale[0];
			m_localTr.m_scale.y = node.scale[1];
			m_localTr.m_scale.z = node.scale[2];
			scaleSet = true;
		}
		if (node.rotation.size())
		{
			m_localTr.m_orientation.w = node.rotation[0];
			m_localTr.m_orientation.x = node.rotation[1];
			m_localTr.m_orientation.y = node.rotation[2];
			m_localTr.m_orientation.z = node.rotation[3];
			orientationSet = true;
		}

		if (node.matrix.size())
		{
			// TODO: Decompose to get position
			if (!locationSet)
			{
			}
			// TODO: Decompose to get scale
			if (!scaleSet)
			{
			}
			// TODO: Decompose to get orientation
			if (!orientationSet)
			{
			}
		}
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