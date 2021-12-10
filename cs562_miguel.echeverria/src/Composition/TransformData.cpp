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

	glm::mat4 TransformData::get_inv_model_mtx() const
	{
		return get_inv_scale_mtx() * get_inv_rotation_mtx() * get_inv_translation_mtx();
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


	glm::mat4 TransformData::get_inv_translation_mtx() const
	{
		return glm::translate(glm::mat4(1.0f), -m_position);
	}

	glm::mat4 TransformData::get_inv_scale_mtx() const
	{
		// Avoid nan
		if (glm::any(glm::epsilonEqual(m_scale, glm::vec3(0.0f, 0.0f, 0.0f), FLT_EPSILON)))
		{
			std::cout << "WARNING: Trying to get the inverse matrix of a 0 scale\n";
			return glm::mat4(1.0f);
		}

		return glm::scale(glm::mat4(1.0f), 1.0f / m_scale);
	}

	glm::mat4 TransformData::get_inv_rotation_mtx() const
	{
		return glm::toMat4(glm::inverse(m_orientation));
	}


	// Concatenates the given local transform with its parent world transform, to find the world data (this transform)
	void TransformData::concatenate(const TransformData& localTr, const TransformData& parentWorldTr)
	{
		m_scale = parentWorldTr.m_scale * localTr.m_scale;
		m_orientation = parentWorldTr.m_orientation * localTr.m_orientation;
		m_position = parentWorldTr.m_position + parentWorldTr.m_orientation * (parentWorldTr.m_scale * localTr.m_position);
	}

	// Takes this transform to local space given its world data, and its parent's world data
	void TransformData::inverse_concatenate(const TransformData& worldTr, const TransformData& parentWorldTr)
	{
		float invScaleX = parentWorldTr.m_scale.x > FLT_EPSILON ? (1.0f / parentWorldTr.m_scale.x) : 0.001f;
		float invScaleY = parentWorldTr.m_scale.y > FLT_EPSILON ? (1.0f / parentWorldTr.m_scale.y) : 0.001f;
		float invScaleZ = parentWorldTr.m_scale.z > FLT_EPSILON ? (1.0f / parentWorldTr.m_scale.z) : 0.001f;
		glm::vec3 invScale = glm::vec3(invScaleX, invScaleY, invScaleZ);

		glm::quat invRotation = glm::inverse(parentWorldTr.m_orientation);

		m_scale = invScale * worldTr.m_scale;
		m_orientation = invRotation * worldTr.m_orientation;
		m_position = invScale * (invRotation * (worldTr.m_position - parentWorldTr.m_position));
	}
}