/**
* @file BlendingCore.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Contains the core functions that are used throughout the rest of
*		 the blending API.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "BlendingCore.h"
#include "Animation/Animation.h"
#include "Math/Interpolation/InterpolationFunctions.h"


namespace cs460
{
	void produce_pose(Animation* anim, AnimPose& pose, float time)
	{
		// For each channel
		for (int i = 0; i < anim->m_channels.size(); ++i)
		{
			// Get/Create the pose data of the joint this channel refers to
			AnimationChannel& channel = anim->m_channels[i];
			std::pair<TransformData, unsigned char>& poseJoint = pose[channel.m_targetNodeIdx];


			// Set the pose data for the current joint depending on the target property
			if (channel.m_targetProperty == "translation")
			{
				poseJoint.first.m_position = anim->sample_vec3(time, i);
				poseJoint.second |= (int)TargetProperty::TRANSLATION;
			}
			else if (channel.m_targetProperty == "scale")
			{
				poseJoint.first.m_scale = anim->sample_vec3(time, i);
				poseJoint.second |= (int)TargetProperty::SCALE;
			}
			else
			{
				poseJoint.first.m_orientation = anim->sample_quat(time, i);
				poseJoint.second |= (int)TargetProperty::ROTATION;
			}
		}
	}


	void blend_pose_lerp(const AnimPose& startPose, const AnimPose& endPose, AnimPose& resultPose, float blendParam, BlendMask* blendMask)
	{
		// We will ignore the blend mask for now

		// For each joint in the start pose
		for (auto& startJoint : startPose)
		{
			// If this joint is not in the end pose, add it without lerping to the result pose
			const auto& foundEndJoint = endPose.find(startJoint.first);
			if (foundEndJoint == endPose.end())
			{
				resultPose[startJoint.first] = startJoint.second;
				continue;
			}

			// Otherwise, perform lerp blending
			unsigned char startPorperties = startJoint.second.second;
			unsigned char endProperties = foundEndJoint->second.second;
			unsigned char overlappingProperties = startPorperties & endProperties;
			
			// Perform translation lerp blending
			if (overlappingProperties == (unsigned char)TargetProperty::TRANSLATION)
			{
				resultPose[startJoint.first].first.m_position = lerp(startJoint.second.first.m_position, foundEndJoint->second.first.m_position, blendParam);
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::TRANSLATION;
			}
			// Perform rotation lerp blending (using nlerp)
			if (overlappingProperties == (unsigned char)TargetProperty::ROTATION)
			{
				resultPose[startJoint.first].first.m_orientation = glm::normalize(glm::slerp(startJoint.second.first.m_orientation, foundEndJoint->second.first.m_orientation, blendParam));
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::ROTATION;
			}
			// Perform scale lerp blending
			if (overlappingProperties == (unsigned char)TargetProperty::SCALE)
			{
				resultPose[startJoint.first].first.m_scale = lerp(startJoint.second.first.m_scale, foundEndJoint->second.first.m_scale, blendParam);
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::SCALE;
			}
		}
	}
}