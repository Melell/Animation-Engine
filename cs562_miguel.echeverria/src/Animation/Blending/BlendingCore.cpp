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
			unsigned char startProperties = startJoint.second.second;
			unsigned char endProperties = foundEndJoint->second.second;
			unsigned char overlappingProperties = startProperties & endProperties;
			
			// Both start and end pose affect translation
			if (overlappingProperties & (unsigned char)TargetProperty::TRANSLATION)
			{
				resultPose[startJoint.first].first.m_position = lerp(startJoint.second.first.m_position, foundEndJoint->second.first.m_position, blendParam);
				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::TRANSLATION;
			}
			else
			{
				// Only start pose affects translation
				if (startProperties & (unsigned char)TargetProperty::TRANSLATION)
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
				if (startProperties & (unsigned char)TargetProperty::ROTATION)
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
				if (startProperties & (unsigned char)TargetProperty::SCALE)
					resultPose[startJoint.first].first.m_scale = startJoint.second.first.m_scale;
				// Only end pose affects rotation
				else if (endProperties & (unsigned char)TargetProperty::SCALE)
					resultPose[startJoint.first].first.m_scale = foundEndJoint->second.first.m_scale;


				resultPose[startJoint.first].second |= (unsigned char)TargetProperty::SCALE;
			}
		}
	}


	void blend_pose_barycentric(const AnimPose& pose0, const AnimPose& pose1, const AnimPose& pose2, float a0, float a1, float a2, AnimPose& resultPose, BlendMask* blendMask)
	{
		// We will ignore the blend mask for now

		// For every joint in the first pose
		for (auto& currElement : pose0)
		{
			const auto& found1 = pose1.find(currElement.first);
			const auto& found2 = pose2.find(currElement.first);
			const auto& pose0Joint = currElement.second;

			if (found1 == pose1.end())
			{
				// CASE 1: Only the first pose joint is available
				if (found2 == pose2.end())
				{
					resultPose[currElement.first] = pose0Joint;
					return;
				}
				// CASE 2: Only the first and third pose joints are available
				else
				{
					unsigned char pose0Properties = pose0Joint.second;
					unsigned char pose2Properties = found2->second.second;
					unsigned char overlappingProperties = pose0Properties & pose2Properties;

					if (overlappingProperties & (unsigned char)TargetProperty::TRANSLATION)
					{

					}
					else if (overlappingProperties & (unsigned char)TargetProperty::ROTATION)
					{

					}
					else if (overlappingProperties & (unsigned char)TargetProperty::SCALE)
					{

					}
				}
			}
			// CASE 3: Only the first and second pose joints are available
			else if (found2 == pose2.end())
			{

			}

			// CASE 4: All pose joints are available

			const auto& pose1Joint = found1->second;
			const auto& pose2Joint = found2->second;

			unsigned char pose0Properties = pose0Joint.second;
			unsigned char pose1Properties = pose1Joint.second;
			unsigned char pose2Properties = pose2Joint.second;
			unsigned char overlappingProperties = pose0Properties & pose1Properties & pose2Properties;

			// The three pose joints affect translation
			if (overlappingProperties & (unsigned char)TargetProperty::TRANSLATION)
			{
				resultPose[currElement.first].first.m_position = a0 * pose0Joint.first.m_position + a1 * pose1Joint.first.m_position + a2 * pose2Joint.first.m_position;
				resultPose[currElement.first].second |= (unsigned char)TargetProperty::TRANSLATION;
			}
			else
			{
				std::cout << "PROBLEM!!!\n";
			}

			// The three pose joints affect rotation
			if (overlappingProperties & (unsigned char)TargetProperty::ROTATION)
			{
				resultPose[currElement.first].first.m_orientation = a0 * pose0Joint.first.m_orientation + a1 * pose1Joint.first.m_orientation + a2 * pose2Joint.first.m_orientation;
				resultPose[currElement.first].second |= (unsigned char)TargetProperty::ROTATION;
			}
			else
			{
				std::cout << "PROBLEM!!!\n";
			}

			// The three pose joints affect scale
			if (overlappingProperties & (unsigned char)TargetProperty::SCALE)
			{
				resultPose[currElement.first].first.m_scale = a0 * pose0Joint.first.m_scale + a1 * pose1Joint.first.m_scale + a2 * pose2Joint.first.m_scale;
				resultPose[currElement.first].second |= (unsigned char)TargetProperty::SCALE;
			}
			else
			{
				std::cout << "PROBLEM!!!\n";
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