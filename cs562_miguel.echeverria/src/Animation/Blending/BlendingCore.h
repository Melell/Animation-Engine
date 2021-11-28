/**
* @file BlendingCore.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Contains the core functions that are used throughout the rest of
*		 the blending API.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct Animation;
	class AnimationReference;


	enum class TargetProperty
	{
		TRANSLATION = 1,
		ROTATION = 2,
		SCALE = 4
	};


	void produce_pose(Animation* anim, AnimPose& pose, float time);
	void blend_pose_lerp(const AnimPose& startPose, const AnimPose& endPose, AnimPose& resultPose, float blendParam, BlendMask* blendMask = nullptr);
	void blend_pose_barycentric(const AnimPose& pose0, const AnimPose& pose1, const AnimPose& pose2, float a0, float a1, float a2, AnimPose& resultPose, BlendMask* blendMask = nullptr);


	// Apply the given pose to the nodes of the skeleton of the given anim component.
	void apply_pose_to_skeleton(const AnimPose& pose, AnimationReference* animComp);
}