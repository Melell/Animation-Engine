/**
* @file Material.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the necessary data to render a primitive in different ways.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Texture.h"


namespace cs460
{
	struct Material
	{
		glm::vec4 m_baseColor;
		Texture m_baseColorTex;
		bool m_usesTexture;
		//bool m_doubleSided;
	};
}