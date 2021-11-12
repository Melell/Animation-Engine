/**
* @file QuaternionHelpers.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Contains helper functions related to quaternions.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "QuaternionHelpers.h"


namespace cs460
{
	glm::quat quat_from_orthonormal_basis(const glm::vec3& t, const glm::vec3& b, const glm::vec3& n)
	{
        float T = t.x + n.y + b.z;
        float s;
        glm::quat result;
        if (T > 0)
        {
            float s = sqrt(T + 1) * 2.f;
            result.x = (b.y - n.z) / s;
            result.y = (t.z - b.x) / s;
            result.z = (n.x - t.y) / s;
            result.w = 0.25f * s;
        }
        else if (t.x > n.y && t.x > b.z)
        {
            s = sqrt(1 + t.x - n.y - b.z) * 2;
            result.x = 0.25f * s;
            result.y = (n.x + t.y) / s;
            result.z = (t.z + b.x) / s;
            result.w = (b.y - n.z) / s;
        }
        else if (n.y > b.z)
        {
            s = sqrt(1 + n.y - t.x - b.z) * 2;
            result.x = (n.x + t.y) / s;
            result.y = 0.25f * s;
            result.z = (b.y + n.z) / s;
            result.w = (n.z - b.y) / s;
        }
        else
        {
            s = sqrt(1 + b.z - t.x - n.y) * 2;
            result.x = (t.z + b.x) / s;
            result.y = (b.y + n.z) / s;
            result.z = 0.25f * s;
            result.w = (n.x - t.y) / s;
        }

        return result;
	}
}