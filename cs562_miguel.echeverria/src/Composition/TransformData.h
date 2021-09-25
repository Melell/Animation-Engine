/**
* @file TransformData.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the data for a transform.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct TransformData
	{
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::quat m_orientation;
	};
}