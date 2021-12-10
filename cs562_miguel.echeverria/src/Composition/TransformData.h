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
		glm::mat4 get_inv_model_mtx() const;

		glm::mat4 get_translation_mtx() const;
		glm::mat4 get_scale_mtx() const;
		glm::mat4 get_rotation_mtx() const;

		glm::mat4 get_inv_translation_mtx() const;
		glm::mat4 get_inv_scale_mtx() const;
		glm::mat4 get_inv_rotation_mtx() const;

		// Concatenates the given local transform with its parent world transform, to find the world data (this transform)
		void concatenate(const TransformData& localTr, const TransformData& parentWorldTr);

		// Takes this transform to local space given its world data, and its parent's world data
		void inverse_concatenate(const TransformData& worldTr, const TransformData& parentWorldTr);


		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
		glm::quat m_orientation{ 1.0f, 0.0f, 0.0f, 0.0f };
	};
}