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
		glm::mat4 get_model_mtx() const;
		glm::mat4 get_translation_mtx() const;
		glm::mat4 get_scale_mtx() const;
		glm::mat4 get_rotation_mtx() const;

		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
		glm::quat m_orientation{ 1.0f, 0.0f, 0.0f, 0.0f };
		//glm::vec3 m_orientation{0.0f, 0.0f, 0.0f};
	};
}