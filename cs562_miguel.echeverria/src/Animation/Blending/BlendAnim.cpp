/**
* @file BlendAnim.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Blend node that contains an animation from which a pose will be
*		 extracted, and blended with the poses of sibling nodes.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "BlendAnim.h"
#include "BlendingCore.h"
#include "Animation/Animation.h"


namespace cs460
{
	// Produce a pose for the internal animation at the given time and store it in m_pose.
	void BlendAnim::produce_pose(float time)
	{
		float realTime = glm::mod(time, m_animSource->m_duration);
		::cs460::produce_pose(m_animSource, m_pose, realTime);
	}
}