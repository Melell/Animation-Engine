/**
* @file Scene.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the implementation of the scene singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Graphics/Renderer.h"
#include "GUI/Editor.h"
#include <GLFW/glfw3.h>		// Input


namespace cs460
{
	Scene::Scene()
		:	m_root(nullptr)
	{

	}
	
	Scene::~Scene()
	{
		clear();
	}


	// System management functions
	bool Scene::initialize()
	{
		clear();
		m_root = new SceneNode("ROOT");
		return true;
	}

	// Update the scene node's world position
	void Scene::update()
	{
		if (m_root == nullptr)
			return;
		

		// TODO: Put this in another place
		if (glfwGetKey(Renderer::get_instance().get_window().get_handle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
			glfwGetKey(Renderer::get_instance().get_window().get_handle(), GLFW_KEY_R) == GLFW_PRESS)
		{
			clear();
			Editor::get_instance().get_state().m_selectedNode = m_root;
			return;
		}

		// Special case, set the root's world transform as the local transform
		m_root->m_worldTr = m_root->m_localTr;

		// Update its childs
		const std::vector<SceneNode*>& children = m_root->get_children();
		for (int i = 0; i < children.size(); ++i)
			update_node(children[i]);
	}

	void Scene::close()
	{
		clear();

		if (m_root != nullptr)
		{
			delete m_root;
			m_root = nullptr;
		}
	}

	
	Scene& Scene::get_instance()
	{
		static Scene instance;
		return instance;
	}

	// Delete all the nodes in the scene graph except the root
	void Scene::clear()
	{
		if (m_root == nullptr)
			return;

		// Delete all the children
		const std::vector<SceneNode*>& children = m_root->get_children();
		for (int i = 0; i < children.size(); ++i)
			delete_tree(children[i]);
	}

	SceneNode* Scene::get_root() const
	{
		return m_root;
	}

	EditorCamera& Scene::get_camera()
	{
		return m_camera;
	}

	std::vector<std::unordered_map<unsigned, SceneNode*>>& Scene::get_all_model_nodes()
	{
		return m_modelNodes;
	}

	std::unordered_map<unsigned, SceneNode*>& Scene::get_model_inst_nodes(unsigned instanceId)
	{
		return m_modelNodes[instanceId];
	}

	// Recursive function to update the transform of the provided node
	void Scene::update_node(SceneNode* node)
	{
		if (node == nullptr)
			return;

		// Update the world transform of the current node (it's parent world tr is already updated)
		if (SceneNode* parent = node->get_parent())
		{
			node->m_worldTr.m_scale = parent->m_worldTr.m_scale * node->m_localTr.m_scale;
			node->m_worldTr.m_orientation = parent->m_worldTr.m_orientation * node->m_localTr.m_orientation;
			node->m_worldTr.m_position = parent->m_worldTr.m_position + parent->m_worldTr.m_orientation * (parent->m_worldTr.m_scale * node->m_localTr.m_position);
		}

		// Update all its childs
		const std::vector<SceneNode*>& children = node->get_children();
		for (int i = 0; i < children.size(); ++i)
			update_node(children[i]);
	}

	// Recursive function to free the memory of all the nodes
	void Scene::delete_tree(SceneNode* node, bool clearParentChildren)
	{
		SceneNode* parent = node->get_parent();
		delete_tree_internal(node);

		if (parent != nullptr && clearParentChildren)
			parent->m_children.clear();
	}

	void Scene::delete_tree_internal(SceneNode* node)
	{
		if (node == nullptr)
			return;

		std::vector<SceneNode*>& children = node->m_children;
		for (auto it = children.begin(); it != children.end();)
		{
			//std::cout << "DELETE TREE\n";
			delete_tree_internal(*it);
			it = children.erase(it);
		}

		delete node;
	}
}