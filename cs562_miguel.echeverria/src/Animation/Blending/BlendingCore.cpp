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
		// Please note that blendParam should already be normalized in the range [0, 1]

		// Generate all the keys given by the start pose
		for (auto& startJoint : startPose)
			resultPose[startJoint.first].second |= startJoint.second.second;

		// Generate all the keys given by the end pose
		for (auto& endJoint : endPose)
			resultPose[endJoint.first].second |= endJoint.second.second;


		// For each joint that should be interpolated
		for (auto& resultJoint : resultPose)
		{
			const auto& foundStartJoint = startPose.find(resultJoint.first);
			const auto& foundEndJoint = endPose.find(resultJoint.first);

			float posParam = blendParam;
			float rotParam = blendParam;
			float scaleParam = blendParam;

			// If the joint is not in the start pose
			if (foundStartJoint == startPose.end())
			{
				resultJoint.second = foundEndJoint->second;
				continue;
			}

			// Else if the joint is not in the end pose
			if (foundEndJoint == endPose.end())
			{
				resultJoint.second = foundStartJoint->second;
				continue;
			}

			// Else, the joint is on both poses
			// Find the properties of the current joint that are affected by both poses
			unsigned char startProperties = foundStartJoint->second.second;
			unsigned char endProperties = foundEndJoint->second.second;
			unsigned char overlappingProperties = startProperties & endProperties;

			// If they don't overlap in translation
			if (!(overlappingProperties & (unsigned char)TargetProperty::TRANSLATION))
			{
				// If only start pose affects translation
				if (startProperties & (unsigned char)TargetProperty::TRANSLATION)
					posParam = 0.0f;
				// If only end pose affects translation
				else if (endProperties & (unsigned char)TargetProperty::TRANSLATION)
					posParam = 1.0f;
			}

			// If they don't overlap in rotation
			if (!(overlappingProperties & (unsigned char)TargetProperty::ROTATION))
			{
				// If only start pose affects translation
				if (startProperties & (unsigned char)TargetProperty::ROTATION)
					rotParam = 0.0f;
				// If only end pose affects translation
				else if (endProperties & (unsigned char)TargetProperty::ROTATION)
					rotParam = 1.0f;
			}

			// If they don't overlap in scale
			if (!(overlappingProperties & (unsigned char)TargetProperty::SCALE))
			{
				// If only start pose affects translation
				if (startProperties & (unsigned char)TargetProperty::SCALE)
					scaleParam = 0.0f;
				// If only end pose affects translation
				else if (endProperties & (unsigned char)TargetProperty::SCALE)
					scaleParam = 1.0f;
			}

			// Interpolate based on current parameters (slerp for rotations)
			resultJoint.second.first.m_position = lerp(foundStartJoint->second.first.m_position, foundEndJoint->second.first.m_position, posParam);
			resultJoint.second.first.m_orientation = glm::normalize(glm::slerp(foundStartJoint->second.first.m_orientation, foundEndJoint->second.first.m_orientation, rotParam));
			resultJoint.second.first.m_scale = lerp(foundStartJoint->second.first.m_scale, foundEndJoint->second.first.m_scale, scaleParam);
		}
	}


	void blend_pose_barycentric(const AnimPose& pose0, const AnimPose& pose1, const AnimPose& pose2, float a0, float a1, float a2, AnimPose& resultPose, BlendMask* blendMask)
	{
		// We will ignore the blend mask for now

		// Generate all the keys given by pose0
		for (auto& jointPose0 : pose0)
			resultPose[jointPose0.first].second |= jointPose0.second.second;

		// Generate all the keys given by pose1
		for (auto& jointPose1 : pose1)
			resultPose[jointPose1.first].second |= jointPose1.second.second;

		// Generate all the keys given by pose2
		for (auto& jointPose2 : pose2)
			resultPose[jointPose2.first].second |= jointPose2.second.second;


		// For each joint that should be interpolated
		for (auto& resultJoint : resultPose)
		{
			const auto& foundJoint0 = pose0.find(resultJoint.first);
			const auto& foundJoint1 = pose1.find(resultJoint.first);
			const auto& foundJoint2 = pose2.find(resultJoint.first);

			float posParams[3] = { a0, a1, a2 };
			float rotParams[3] = { a0, a1, a2 };
			float scaleParams[3] = { a0, a1, a2 };

			// If the joint is not in pose0
			if (foundJoint0 == pose0.end())
			{
				// If the joint is also not in pose 1 (only in pose 2)
				if (foundJoint1 == pose1.end())
				{
					// Use the joint from pose 2
					resultJoint.second = foundJoint2->second;
					continue;
				}
				// If the joint is also not in pose 2 (only in pose 1)
				else if (foundJoint2 == pose2.end())
				{
					// Use the joint from pose 1
					resultJoint.second = foundJoint1->second;
					continue;
				}
			}
			// Else if the joint is only in pose 0
			else if (foundJoint1 == pose1.end() && foundJoint2 == pose2.end())
			{
				// Use the joint from pose 0
				resultJoint.second = foundJoint0->second;
				continue;
			}


			// At this point, either the three joints are there, or only one is missing

			// Only joint 0 is missing
			if (foundJoint0 == pose0.end())
			{	
				posParams[0] = 0.0f;
				rotParams[0] = 0.0f;
				scaleParams[0] = 0.0f;
			}
			// Only joint 1 is missing
			else if (foundJoint1 == pose1.end())
			{
				posParams[1] = 0.0f;
				rotParams[1] = 0.0f;
				scaleParams[1] = 0.0f;
			}
			// Only joint 2 is missing
			else if (foundJoint2 == pose2.end())
			{
				posParams[2] = 0.0f;
				rotParams[2] = 0.0f;
				scaleParams[2] = 0.0f;
			}
			// None of the joints is missing
			else
			{
				// Find the properties of the current joint that are affected by the three poses
				unsigned char joint0Properties = foundJoint0->second.second;
				unsigned char joint1Properties = foundJoint1->second.second;
				unsigned char joint2Properties = foundJoint2->second.second;

				// If pose0 doesn't affect translation
				if (!(joint0Properties & (unsigned char)TargetProperty::TRANSLATION))
					posParams[0] = 0.0f;
				// If pose1 doesn't affect translation
				if (!(joint1Properties & (unsigned char)TargetProperty::TRANSLATION))
					posParams[1] = 0.0f;
				// If pose2 doesn't affect translation
				if (!(joint2Properties & (unsigned char)TargetProperty::TRANSLATION))
					posParams[2] = 0.0f;

				// If pose0 doesn't affect rotation
				if (!(joint0Properties & (unsigned char)TargetProperty::ROTATION))
					rotParams[0] = 0.0f;
				// If pose1 doesn't affect rotation
				if (!(joint1Properties & (unsigned char)TargetProperty::ROTATION))
					rotParams[1] = 0.0f;
				// If pose2 doesn't affect rotation
				if (!(joint2Properties & (unsigned char)TargetProperty::ROTATION))
					rotParams[2] = 0.0f;

				// If pose0 doesn't affect scale
				if (!(joint0Properties & (unsigned char)TargetProperty::SCALE))
					scaleParams[0] = 0.0f;
				// If pose1 doesn't affect scale
				if (!(joint1Properties & (unsigned char)TargetProperty::SCALE))
					scaleParams[1] = 0.0f;
				// If pose2 doesn't affect scale
				if (!(joint2Properties & (unsigned char)TargetProperty::SCALE))
					scaleParams[2] = 0.0f;
			}

			// Interpolate based on current parameters (barycentric interpolation)
			resultJoint.second.first.m_position = posParams[0] * foundJoint0->second.first.m_position +
												  posParams[1] * foundJoint1->second.first.m_position +
												  posParams[2] * foundJoint2->second.first.m_position;

			resultJoint.second.first.m_orientation = rotParams[0] * foundJoint0->second.first.m_orientation +
													 rotParams[1] * foundJoint1->second.first.m_orientation +
													 rotParams[2] * foundJoint2->second.first.m_orientation;

			resultJoint.second.first.m_scale = scaleParams[0] * foundJoint0->second.first.m_scale +
											   scaleParams[1] * foundJoint1->second.first.m_scale +
											   scaleParams[2] * foundJoint2->second.first.m_scale;
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