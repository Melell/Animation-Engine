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
	void SceneNode::create_child(const std::string& name)
	{
		SceneNode* newNode = new SceneNode(name);
		newNode->m_parent = this;
		m_children.push_back(newNode);
	}

	SceneNode* SceneNode::get_parent() const
	{
		return m_parent;
	}

	const std::vector<SceneNode*>& SceneNode::get_children() const
	{
		return m_children;
	}
}