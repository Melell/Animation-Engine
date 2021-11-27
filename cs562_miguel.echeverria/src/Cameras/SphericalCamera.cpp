/**
* @file SphericalCamera.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Contains the logic for a spherical camera that orbits around a given focal point.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "SphericalCamera.h"
#include "Composition/SceneNode.h"


namespace cs460
{
	// Update the view matrix with the current parameters
	void SphericalCamera::update_view_mtx()
	{

	}


	// Getters for the camera controlling parameters
	SceneNode* SphericalCamera::get_focal_node() const
	{
		return m_focalNode;
	}

	glm::vec3 SphericalCamera::get_focal_point() const
	{
		return m_focalPoint;
	}

	float SphericalCamera::get_radius() const
	{
		return m_radius;
	}

	float SphericalCamera::get_y_angle() const
	{
		return m_yAngle;
	}

	float SphericalCamera::get_polar_angle() const
	{
		return m_polarAngle;
	}


	// Setters for the camera controlling parameters
	void SphericalCamera::set_focal_node(SceneNode* node)
	{
		m_focalNode = node;
	}

	void SphericalCamera::set_focal_point(const glm::vec3& point)
	{
		m_focalPoint = point;
	}

	void SphericalCamera::set_radius(float radius)
	{
		m_radius = radius;
	}

	void SphericalCamera::set_y_angle(float yAngle)
	{
		m_yAngle = yAngle;
	}

	void SphericalCamera::set_polar_angle(float polarAngle)
	{
		m_polarAngle = polarAngle;
	}


	// Actual logic for the spherical camera goes here
	void SphericalCamera::camera_logic()
	{
		
	}
}