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

#include "Composition/TransformData.h"


namespace cs460
{
	struct Animation;

	enum class TargetProperty
	{
		TRANSLATION = 1,
		ROTATION = 2,
		SCALE = 4
	};

	// <NodeIdx, pair<NodeTransform, BitMaskForVQS> >
	typedef std::unordered_map<unsigned, std::pair<TransformData, unsigned char> > AnimPose;

	// <NodeIdx, BlendParam>
	typedef std::unordered_map<unsigned, float> BlendMask;


	void produce_pose(Animation* anim, AnimPose& pose, float time);
	void blend_pose_lerp(const AnimPose& startPose, const AnimPose& endPose, AnimPose& resultPose, float blendParam, BlendMask* blendMask = nullptr);
	void BlendPoseBarycentric(const AnimPose& pose0, const AnimPose& pose1, const AnimPose& pose2, float a0, float a1, float a2, AnimPose& resultPose, BlendMask* blendMask = nullptr);
}