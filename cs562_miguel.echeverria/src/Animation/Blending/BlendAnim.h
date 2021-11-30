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
	struct Animation;


	struct BlendAnim : public IBlendNode
	{
		Animation* m_animSource = nullptr;
		//BlendMask m_blendMask;

		
		// Sets the animation component owner
		BlendAnim(AnimationReference* animCompOwner);
		
		// Produce a pose for the internal animation at the given time and store it in m_pose.
		void produce_pose(float time) override;
	};
}