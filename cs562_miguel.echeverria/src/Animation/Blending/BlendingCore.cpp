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
#include "Components/Animation/AnimationReference.h"
#include "Composition/Scene.h"
#include "Composition/SceneNode.h"
#include "Components/Models/ModelInstance.h"


namespace cs460
{
	void produce_pose(Animation* anim, AnimPose& pose, float time)
	{
		// Clear any remaining pose data
		pose.clear();

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
			
			// Both start and end pose affect translation
			if (overlappingProperties & (unsigned char)TargetProperty::TRANSLATION)
			{
				resultPose[startJoint.first].first.m_position = lerp(startJoint.second.first.m_position, foundEndJoint->second.first.m_position, blendParam);
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::TRANSLATION;
			}
			else
			{
				// Only start pose affects translation
				if (startPorperties & (unsigned char)TargetProperty::TRANSLATION)
					resultPose[startJoint.first].first.m_position = startJoint.second.first.m_position;
				// Only end pose affects translation
				else if (endProperties & (unsigned char)TargetProperty::TRANSLATION)
					resultPose[startJoint.first].first.m_position = foundEndJoint->second.first.m_position;


				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::TRANSLATION;
			}

			// Both start and end pose affect rotation (use nlerp)
			if (overlappingProperties & (unsigned char)TargetProperty::ROTATION)
			{
				resultPose[startJoint.first].first.m_orientation = glm::normalize(glm::slerp(startJoint.second.first.m_orientation, foundEndJoint->second.first.m_orientation, blendParam));
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::ROTATION;
			}
			else
			{
				// Only start pose affects rotation
				if (startPorperties & (unsigned char)TargetProperty::ROTATION)
					resultPose[startJoint.first].first.m_orientation = startJoint.second.first.m_orientation;
				// Only end pose affects rotation
				else if (endProperties & (unsigned char)TargetProperty::ROTATION)
					resultPose[startJoint.first].first.m_orientation = foundEndJoint->second.first.m_orientation;


				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::ROTATION;
			}

			// Both start and end pose affect scale
			if (overlappingProperties & (unsigned char)TargetProperty::SCALE)
			{
				resultPose[startJoint.first].first.m_scale = lerp(startJoint.second.first.m_scale, foundEndJoint->second.first.m_scale, blendParam);
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::SCALE;
			}
			else
			{
				// Only start pose affects scale
				if (startPorperties & (unsigned char)TargetProperty::SCALE)
					resultPose[startJoint.first].first.m_scale = startJoint.second.first.m_scale;
				// Only end pose affects rotation
				else if (endProperties & (unsigned char)TargetProperty::SCALE)
					resultPose[startJoint.first].first.m_scale = foundEndJoint->second.first.m_scale;


				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::SCALE;
			}
		}
	}



	// Apply the given pose to the nodes of the skeleton of the given anim component.
	void apply_pose_to_skeleton(const AnimPose& pose, AnimationReference* animComp)
	{
		Scene& scene = Scene::get_instance();

		ModelInstance* modelInst = animComp->get_owner()->get_component<ModelInstance>();
		auto& modelInstNodes = scene.get_model_inst_nodes(modelInst->get_instance_id());

		// For every joint in the pose
		for (auto& joint : pose)
		{
			SceneNode* jointNode = modelInstNodes[joint.first];

			// If translation has been modified, apply it
			if (joint.second.second & (unsigned char)TargetProperty::TRANSLATION)
			{
				jointNode->m_localTr.m_position = joint.second.first.m_position;
			}
			// If orientation has been modified, apply it
			if (joint.second.second & (unsigned char)TargetProperty::ROTATION)
			{
				jointNode->m_localTr.m_orientation = joint.second.first.m_orientation;
			}
			// If scale has been modified, apply it
			if (joint.second.second & (unsigned char)TargetProperty::SCALE)
			{
				jointNode->m_localTr.m_scale = joint.second.first.m_scale;
			}
		}
	}
}