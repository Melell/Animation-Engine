/**
* @file BlendTree.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Container for the actual hierarchy of the blend tree.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "BlendTree.h"
#include "IBlendNode.h"


namespace cs460
{
	// Default ctor and dtor
	/*BlendTree::BlendTree()
	{
	}

	BlendTree::~BlendTree()
	{
		clear();
	}


	void BlendTree::produce_pose(float time)
	{
		if (m_root == nullptr)
			return;

		m_root->produce_pose(time);
	}

	AnimPose& BlendTree::get_current_pose()
	{
		if (m_root == nullptr)
			throw "ERROR: Calling get_current_pose on an empty tree\n";

		return m_root->m_pose;
	}


	IBlendNode* BlendTree::get_root()
	{
		return m_root;
	}


	// Returns true if the root is null
	bool BlendTree::empty()
	{
		return m_root == nullptr;
	}


	void BlendTree::clear()
	{
		clear_node(m_root);
		m_root = nullptr;
	}

	void BlendTree::clear_node(IBlendNode* node)
	{
		if (node == nullptr)
			return;

		// Clear each child
		for (int i = 0; i < node->m_children.size(); ++i)
			clear_node(node->m_children[i]);

		node->m_children.clear();
		delete node;
	}


	void BlendTree::internalFromJson(json& value)
	{

	}
	void BlendTree::internalToJson(json& value)
	{

	}*/
}