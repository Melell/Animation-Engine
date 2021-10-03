/**
* @file TransformData.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the implementation for transform data functions.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "TransformData.h"


namespace cs460
{
	glm::mat4 TransformData::get_model_mtx() const
	{
		return get_translation_mtx() * get_rotation_mtx() * get_scale_mtx();
	}

	glm::mat4 TransformData::get_translation_mtx() const
	{
		return glm::translate(glm::mat4(1.0f), m_position);
	}

	glm::mat4 TransformData::get_scale_mtx() const
	{
		return glm::scale(glm::mat4(1.0f), m_scale);
	}

	glm::mat4 TransformData::get_rotation_mtx() const
	{
		return glm::toMat4(m_orientation);
	}
}