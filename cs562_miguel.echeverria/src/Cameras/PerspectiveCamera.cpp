/**
* @file PerspectiveCamera.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the necessary data to form a perspective projection matrix.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "PerspectiveCamera.h"


namespace cs460
{
	PerspectiveCamera::PerspectiveCamera()
		:	m_yFov(45.0f),
			m_aspectRatio(16.0f / 9.0f),
			m_near(0.1f),
			m_far(1000.0f)
			
	{
		update_projection_mtx();
	}

	PerspectiveCamera::PerspectiveCamera(float yFov, float aspectRatio, float near, float far)
		:	m_yFov(yFov),
			m_aspectRatio(aspectRatio),
			m_near(near),
			m_far(far)
	{
		update_projection_mtx();
	}


	// Meant to be implemented by child classes(editor cam, spherical cam etc)
	void PerspectiveCamera::update_view_mtx()
	{
	}


	void PerspectiveCamera::update_projection_mtx()
	{
		m_projectionMtx = glm::perspective(m_yFov, m_aspectRatio, m_near, m_far);
	}


	// Getters for the camera parameters
	float PerspectiveCamera::get_y_fov() const
	{
		return m_yFov;
	}

	float PerspectiveCamera::get_aspect_ratio() const
	{
		return m_aspectRatio;
	}

	float PerspectiveCamera::get_near() const
	{
		return m_near;
	}

	float PerspectiveCamera::get_far() const
	{
		return m_far;
	}


	// Setters for the camera parameters
	void PerspectiveCamera::set_y_fov(float yFov)
	{
		m_yFov = yFov;
	}

	void PerspectiveCamera::set_aspect_ratio(float aspectRatio)
	{
		m_aspectRatio = aspectRatio;
	}

	void PerspectiveCamera::set_near(float near)
	{
		m_near = near;
	}

	void PerspectiveCamera::set_far(float far)
	{
		m_far = far;
	}


	// Meant to be overriden by specific cameras
	void PerspectiveCamera::camera_logic()
	{
	}
}