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

	void Scene::update()
	{
		// TODO: Update the scene graph (concatenate transforms)
	}

	void Scene::close()
	{
		clear();
	}

	
	Scene& Scene::get_instance()
	{
		static Scene instance;
		return instance;
	}

	// Delete all the nodes in the scene graph
	void Scene::clear()
	{
		delete_tree(m_root);
		m_root = nullptr;
	}

	SceneNode* Scene::get_root() const
	{
		return m_root;
	}

	// Recursive function to free the memory of all the nodes
	void Scene::delete_tree(SceneNode* node)
	{
		const std::vector<SceneNode*>& children = node->get_children();
		for (int i = 0; i < children.size(); ++i)
			delete_tree(children[i]);

		delete node;
	}
}