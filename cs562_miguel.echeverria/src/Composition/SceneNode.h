/**
* @file SceneNode.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Declaration of the class that represents a node in the scene
*		 graph (like a gameobject), already containing transform information.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "TransformData.h"


namespace cs460
{
	class SceneNode
	{
	public:
	
	private:
		TransformData m_localTr;
		TransformData m_worldTr;
	};
}