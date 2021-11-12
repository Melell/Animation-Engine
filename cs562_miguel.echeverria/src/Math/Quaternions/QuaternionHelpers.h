/**
* @file QuaternionHelpers.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Contains helper functions related to quaternions.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	glm::quat quat_from_orthonormal_basis(const glm::vec3& t, const glm::vec3& b, const glm::vec3& n);
}