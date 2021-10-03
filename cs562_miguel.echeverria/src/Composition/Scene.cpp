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

		// Special case, set the root's model to world matrix as its model to local
		m_root->m_modelToLocalMtx = m_root->m_localTr.get_model_mtx();
		m_root->m_modelToWorldMtx = m_root->m_modelToLocalMtx;

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

	// Recursive function to update the transform of the provided node
	void Scene::update_node(SceneNode* node)
	{
		if (node == nullptr)
			return;

		// Update the model to local space matrix
		node->m_modelToLocalMtx = node->m_localTr.get_model_mtx();

		// Update the model to world space matrix (the parent's model to world is already updated)
		if (node->get_parent())
			node->m_modelToWorldMtx = node->get_parent()->m_modelToWorldMtx * node->m_modelToLocalMtx;

		// Update all its childs
		const std::vector<SceneNode*>& children = node->get_children();
		for (int i = 0; i < children.size(); ++i)
			update_node(children[i]);
	}

	// Recursive function to free the memory of all the nodes
	void Scene::delete_tree(SceneNode* node)
	{
		if (node == nullptr)
			return;

		const std::vector<SceneNode*>& children = node->get_children();
		for (int i = 0; i < children.size(); ++i)
			delete_tree(children[i]);

		delete node;
	}
}