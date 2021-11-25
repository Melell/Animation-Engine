/**
* @file Blend2D.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Type of blend node that allows for blending of multiple animations in
*		 a 2 dimensional space.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Blend2D.h"


namespace cs460
{
	void Blend2D::produce_pose(float time)
	{

	}


	void Blend2D::blend_children(float time)
	{
		for (int i = 0; i < m_children.size(); ++i)
		{
			m_children[i]->produce_pose(time);
		}
	}
}