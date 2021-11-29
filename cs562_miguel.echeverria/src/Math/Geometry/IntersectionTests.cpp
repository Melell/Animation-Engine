/**
* @file IntersectionTests.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Contains helpful intersection checking functions.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "IntersectionTests.h"
#include "Geometry.h"


namespace cs460
{
    // Returns -1 if ray doesn't intersect aabb or is a false positive.
    // Returns the time of intersection otherwise. (function taken from cs350)
	float ray_vs_aabb(const Ray& ray, const AABB& aabb)
	{
        float tMin = 0.0f;
        float tMax = FLT_MAX;

        // For each slab
        for (int i = 0; i < 3; ++i)
        {
            // If the ray's dir is parallel to the current slab (avoid division by 0)
            if (glm::epsilonEqual(ray.m_dir[i], 0.0f, FLT_EPSILON))
            {
                // If the ray is parallel and not inside this slab, then there is no ray intersection
                if (ray.m_origin[i] < aabb.m_min[i] || ray.m_origin[i] > aabb.m_max[i])
                    return -1.0f;
            }
            // If the ray is not parallel to the current slab
            else
            {
                float tEnter = (aabb.m_min[i] - ray.m_origin[i]) / ray.m_dir[i];   // Intersection t with first slab
                float tExit = (aabb.m_max[i] - ray.m_origin[i]) / ray.m_dir[i];    // Intersection t with second slab

                // Just in case, make sure tEnter represents the t intersection with first slab of current axis
                if (tEnter > tExit)
                    std::swap(tEnter, tExit);

                // Update the min and max ts
                if (tEnter > tMin)
                    tMin = tEnter;
                if (tExit < tMax)
                    tMax = tExit;

                // And if you reach one axis where this holds, that means that one
                // axis' slab doesn't intersect, so the ray doesn't intersect
                if (tMin > tMax)
                    return -1.0f;
            }
        }

        // Regular intersection, enter and exit
        if (tMin > 0.0f)
            return tMin;

        // Ray starts inside aabb
        return 0.0f;
	}


    // Returns true if the given 2d point is inside a 2d aabb defined by min and max
    bool point_in_aabb_2d(const glm::vec2& point, const glm::vec2& min, const glm::vec2& max)
    {
        if (point.x < min.x || point.x > max.x)
            return false;

        if (point.y < min.y || point.y > max.y)
            return false;

        return true;
    }
}