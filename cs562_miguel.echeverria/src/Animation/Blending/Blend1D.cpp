/**
* @file Blend1D.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Type of blend node that allows for blending of multiple animations in 1 dimension.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Blend1D.h"
#include "BlendingCore.h"


namespace cs460
{
	// loop through the children and find the blend nodes segment whose 
	// blend position (x values) corresponds to the range in which param is contained. 
	// Note: If the children are sorted incrementally by their blend position (x values), 
	// then binary search can be used to speed up the procedure.
	// Note: You could also store the previous frame's segment to start the search tehre
	// as it's highly likely that the param won't change much from frame to frame.
	void Blend1D::find_segment(IBlendNode*& from, IBlendNode*& to)
	{
		// Perform linear search for now. Later this will be a binary search

		float leftClosest = FLT_MAX;
		float rightClosest = FLT_MAX;
		float maxPos = -FLT_MAX;
		float minPos = FLT_MAX;
		int minIdx = 0;
		int maxIdx = 0;

		// For each child
		for (int i = 0; i < m_children.size(); ++i)
		{
			// Compute the difference in blend space
			float diff = m_children[i]->m_blendPos.x - m_blendParam;

			// If current child is to the left of blend param and closer than previous one
			if (diff <= 0.0f && glm::abs(diff) < leftClosest)
			{
				leftClosest = glm::abs(diff);
				from = m_children[i];
			}
			// Else if current child is to the right of blend param and closer than previous one
			else if (diff > 0.0f && glm::abs(diff) < rightClosest)
			{
				rightClosest = glm::abs(diff);
				to = m_children[i];
			}

			// Update the maximum and minimum blend positions
			if (m_children[i]->m_blendPos.x < minPos)
			{
				minPos = m_children[i]->m_blendPos.x;
				minIdx = i;
			}
			if (m_children[i]->m_blendPos.x > maxPos)
			{
				maxPos = m_children[i]->m_blendPos.x;
				maxIdx = i;
			}
		}

		// If the blend parameter was out of the range
		if (m_blendParam >= maxPos)
		{
			from = m_children[maxIdx];
			to = m_children[maxIdx];
		}
		if (m_blendParam <= minPos)
		{
			from = m_children[minIdx];
			to = m_children[minIdx];
		}
	}


	// Produces a poses by blending between its children.
	void Blend1D::produce_pose(float time)
	{
		if (m_children.empty())
			return;

		blend_children(time);
	}


	// Sort the children based on their position in the 1D blend space (smallest x to biggest x)
	void Blend1D::sort_children()
	{
		std::sort(m_children.begin(), m_children.end(), [] (const IBlendNode* lhs, const IBlendNode* rhs) -> bool
		{
			return lhs->m_blendPos.x < rhs->m_blendPos.x;
		});
	}


	// Performs one dimensional blending using the children nodes.
	void Blend1D::blend_children(float time)
	{
		// Find the blend nodes to interpolate
		IBlendNode* from, * to;
		from = to = nullptr;
		find_segment(from, to);

		// Produce the pose from each
		from->produce_pose(time);
		to->produce_pose(time);

		// Normalize the blend parameter for the pose blending
		float normalizedBlendParam = get_normalized_blend_param(from, to);

		// blend the pose into ours
		blend_pose_lerp(from->m_pose, to->m_pose, m_pose, normalizedBlendParam);
	}

	// Normalize the blend parameter to the range [0, 1] (0=at from, 1=at to)
	float Blend1D::get_normalized_blend_param(IBlendNode* from, IBlendNode* to)
	{
		float segmentBlendDist = to->m_blendPos.x - from->m_blendPos.x;
		float normalizedBlendParam = 1.0f;
		if (glm::epsilonNotEqual(segmentBlendDist, 0.0f, FLT_EPSILON))
		{
			float segmentBlendParam = m_blendParam - from->m_blendPos.x;
			normalizedBlendParam = segmentBlendParam / segmentBlendDist;
		}
		normalizedBlendParam = glm::clamp(normalizedBlendParam, 0.0f, 1.0f);
		return normalizedBlendParam;
	}
}