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
		glm::vec3 m_start{-0.5f, 0.0f, 0.0f};
		glm::vec3 m_end{0.5f, 0.0f, 0.0f};
	};


	struct Triangle
	{
		glm::vec3 m_v1{ 0.0f,  0.5f, 0.0f};
		glm::vec3 m_v2{-0.5f, -0.5f, 0.0f};
		glm::vec3 m_v3{ 0.5f, -0.5f, 0.0f};
	};


	struct AABB
	{
		glm::vec3 m_min{-0.5f, -0.5f, -0.5f};
		glm::vec3 m_max{  0.5,  0.5f,  0.5f};
	};


	struct Sphere
	{
		glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
		float m_radius = 0.5f;
	};
}