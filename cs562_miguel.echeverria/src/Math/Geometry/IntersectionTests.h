/**
* @file IntersectionTests.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Contains helpful intersection checking functions.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct Segment;
	struct Ray;
	struct Triangle;
	struct AABB;
	struct Sphere;


	// Returns -1 if ray doesn't intersect aabb or is a false positive.
	// Returns the time of intersection otherwise. (function taken from cs350)
	float ray_vs_aabb(const Ray& ray, const AABB& aabb);


	// Returns true if the given 2d point is inside a 2d aabb defined by min and max
	bool point_in_aabb_2d(const glm::vec2& point, const glm::vec2& min, const glm::vec2& max);
}