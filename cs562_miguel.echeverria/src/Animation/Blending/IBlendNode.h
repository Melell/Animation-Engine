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
	struct IBlendNode
	{
		IBlendNode* m_parent = nullptr;
		std::vector<IBlendNode*> m_children;

		AnimPose m_pose;
	};
}