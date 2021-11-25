/**
* @file IBlendNode.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Interface for the different types of blend nodes. Also contains information
*		 about animation poses.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "IBlendNode.h"
#include "Blend1D.h"
#include "Blend2D.h"
#include "BlendAnim.h"


namespace cs460
{
	void IBlendNode::produce_pose(float time)
	{
	}


	// Create and add a blend node child of the given type
	IBlendNode* IBlendNode::add_child(BlendNodeTypes type)
	{
		IBlendNode* newNode = nullptr;
		
		if (type == BlendNodeTypes::BLEND_1D)
			newNode = new Blend1D;
		else if (type == BlendNodeTypes::BLEND_2D)
			newNode = new Blend2D;
		else if (type == BlendNodeTypes::BLEND_ANIM)
			newNode = new BlendAnim;

		newNode->m_parent = this;
		m_children.push_back(newNode);
		return newNode;
	}

	// Remove the given blend node from the vector of children
	void IBlendNode::remove_child(IBlendNode* child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (child == *it)
			{
				it = m_children.erase(it);
				continue;
			}
		}

		// TODO: Delete the memory? Who is in charge of deleting the memory?
	}


	// Blends the children's poses into this node's pose.
	// Meant to be overriden by Blend1D and Blend2D.
	void IBlendNode::blend_children(float time)
	{
	}
}