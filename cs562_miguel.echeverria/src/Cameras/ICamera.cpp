/**
* @file ICamera.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Base class for all cameras in the game/editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ICamera.h"


namespace cs460
{
	// Call the internal virtual method that handles the camera controls
	void ICamera::update()
	{
		if (m_isActive)
			camera_logic();

		update_view_mtx();
		update_projection_mtx();
	}


	// Get the current position of the camera
	const glm::vec3& ICamera::get_position() const
	{
		return m_position;
	}

	void ICamera::set_position(const glm::vec3 pos)
	{
		m_position = pos;
	}


	// Meant to be implemented by child classes(editor cam, spherical cam etc)
	void ICamera::update_view_mtx()
	{
	}
	const glm::mat4& ICamera::get_view_mtx() const
	{
		return m_viewMtx;
	}


	// Meant to be implemented by child classes (perspective and orthogonal cams)
	void ICamera::update_projection_mtx()
	{
	}

	const glm::mat4& ICamera::get_projection_mtx() const
	{
		return m_projectionMtx;
	}


	// Setter and getter for the boolean that determines whether the camera is updated or not
	void ICamera::set_is_active(bool isActive)
	{
		m_isActive = isActive;
	}

	bool ICamera::get_is_active() const
	{
		return m_isActive;
	}


	// Meant to be overriden by specific cameras
	void ICamera::camera_logic()
	{
	}
}