/**
* @file Blend2D.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Type of blend node that allows for blending of multiple animations in
*		 a 2 dimensional space.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IBlendNode.h"


namespace cs460
{
	struct Blend2D : public IBlendNode
	{
		glm::vec2 m_blendParam;
	};
}