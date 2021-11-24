/**
* @file BlendAnim.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Blend node that contains an animation from which a pose will be
*		 extracted, and blended with the poses of sibling nodes.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IBlendNode.h"


namespace cs460
{
	struct BlendAnim : public IBlendNode
	{
		Animation* m_animSource;
		BlendMask m_blendMask;
		glm::vec2 m_blendPos;		// Only x is used in a 1D blend
	};
}