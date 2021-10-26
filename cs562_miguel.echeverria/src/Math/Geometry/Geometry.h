/**
* @file Geometry.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains helpful geometric structures like segment or aabb.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct Segment
	{
		glm::vec3 m_start;
		glm::vec3 m_end;
	};


	struct Triangle
	{
		glm::vec3 m_v1;
		glm::vec3 m_v2;
		glm::vec3 m_v3;
	};


	struct AABB
	{
		glm::vec3 m_min;
		glm::vec3 m_max;
	};


	struct Sphere
	{
		glm::vec3 m_pos;
		float m_radius;
	};
}