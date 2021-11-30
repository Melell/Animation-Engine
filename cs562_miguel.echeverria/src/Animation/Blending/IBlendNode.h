/**
* @file IBlendNode.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Interface for the different types of blend nodes. Also contains information
*		 about animation poses.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "BlendingCore.h"


namespace cs460
{
	enum class BlendNodeTypes
	{
		BLEND_1D,
		BLEND_2D,
		BLEND_ANIM
	};


	struct IBlendNode
	{
		AnimationReference* m_animCompOwner = nullptr;
		IBlendNode* m_parent = nullptr;
		std::vector<IBlendNode*> m_children;
		AnimPose m_pose;
		glm::vec2 m_blendPos{0.0f, 0.0f};		// Only x is used in a 1D blend


		// Sets the animation component owner
		IBlendNode(AnimationReference* animCompOwner);

		// Create and add a blend node child of the given type
		IBlendNode* add_child(BlendNodeTypes type);

		// Remove the given blend node from the vector of children
		void remove_child(IBlendNode* child);

		// Gets the blended(or not, depending on type of node) pose at time into m_pose
		virtual void produce_pose(float time) = 0;

	private:
		// Blends the children's poses into this node's pose.
		// Meant to be overriden by Blend1D and Blend2D.
		virtual void blend_children(float time);
	};
}