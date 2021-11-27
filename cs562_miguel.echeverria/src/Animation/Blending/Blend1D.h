/**
* @file Blend1D.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Type of blend node that allows for blending of multiple animations in 1 dimension.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IBlendNode.h"


namespace cs460
{
	struct Blend1D : public IBlendNode
	{
		float m_blendParam = 0.0f;


        // Finds the segment in which m_blendParam lies (returns in out-parameters
        // from and to, the blend nodes that form this segment).
        void find_segment(IBlendNode*& from, IBlendNode*& to);

		// Produces a poses by blending between its children.
		void produce_pose(float time) override;

	private:

        // Performs one dimensional blending using the children nodes.
		void blend_children(float time) override;

		// Normalize the blend parameter to the range [0, 1] (0=at from, 1=at to)
		float get_normalized_blend_param(IBlendNode* from, IBlendNode* to);
	};
}