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
#include "Scene.h"
#include <imgui/imgui.h>
#include <gltf/tiny_gltf.h>


namespace cs460
{
	SceneNode::SceneNode(const std::string& name)
		:	m_name(name),
			m_parent(nullptr),
			m_modelRootNode(nullptr),
			m_sourceModel(nullptr)
	{

	}
	SceneNode::~SceneNode()
	{
		// Clear in case there are still components
		delete_all_components();
	}

	// Free all the children of this node
	void SceneNode::delete_all_children()
	{
		for (auto it = m_children.begin(); it != m_children.end();)
		{
			Scene::get_instance().delete_tree(*it, false);
			it = m_children.erase(it);
		}
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

	// Create a new node and add it as this node's child (from name)
	SceneNode* SceneNode::create_child(const std::string& name)
	{
		SceneNode* newNode = new SceneNode(name);
		newNode->m_parent = this;
		m_children.push_back(newNode);
		return newNode;
	}

	// Generates all the data for this scenenode from a gltf node
	void SceneNode::from_node_resource(Model* sourceModel, int nodeIdx, ModelInstance* rootModelInst)
	{
		// Save the model resource and the scene node with the model instance
		m_sourceModel = sourceModel;
		m_modelRootNode = rootModelInst->get_owner();


		// Store the node in the "dictionary"
		auto& modelInstNodes = Scene::get_instance().get_model_inst_nodes(rootModelInst->get_instance_id());
		modelInstNodes[nodeIdx] = this;


		// Save the transform info
		GLTFNode& node = sourceModel->m_nodes[nodeIdx];
		m_localTr.m_position = node.m_localTransform.m_position;
		m_localTr.m_orientation = node.m_localTransform.m_orientation;
		m_localTr.m_scale = node.m_localTransform.m_scale;


		// If this node uses a mesh, save its index in the model's vector of meshes (by generating a mesh renderable for drawing)
		if (node.m_meshIdx >= 0 && node.m_meshIdx < sourceModel->m_meshes.size())
		{
			MeshRenderable* comp = add_component<MeshRenderable>();
			comp->set_mesh_idx(node.m_meshIdx);
		}


		// Create the child nodes
		for (int i = 0; i < node.m_childrenIndices.size(); ++i)
		{
			int childIdx = node.m_childrenIndices[i];
			GLTFNode& childNode = sourceModel->m_nodes[childIdx];
			SceneNode* child = create_child(childNode.m_name);
			child->from_node_resource(sourceModel, childIdx, rootModelInst);
		}
	}

	// Show the components gui
	void SceneNode::on_gui()
	{
		ImGui::Text("Name: %s", m_name.c_str());
		ImGui::Separator();
		ImGui::NewLine();

		show_transforms_gui();

		// Can't add components to the root
		if (Scene::get_instance().get_root() == this)
			return;

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

	SceneNode* SceneNode::get_model_root_node() const
	{
		return m_modelRootNode;
	}

	Model* SceneNode::get_model() const
	{
		return m_sourceModel;
	}


	void SceneNode::show_transforms_gui()
	{
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Local");
			ImGui::DragFloat3("Position##0", glm::value_ptr(m_localTr.m_position));
			ImGui::DragFloat4("Rotation##0", glm::value_ptr(m_localTr.m_orientation));
			ImGui::DragFloat3("Scale##0", glm::value_ptr(m_localTr.m_scale));

			//ImGui::NewLine();
			//ImGui::Separator();
			//ImGui::NewLine();

			//ImGui::Text("World");
			//ImGui::DragFloat3("Position##1", glm::value_ptr(m_worldTr.m_position));
			//ImGui::DragFloat3("Rotation##1", glm::value_ptr(m_worldTr.m_orientation));
			//ImGui::DragFloat3("Scale##1", glm::value_ptr(m_worldTr.m_scale));
		}
	}
}