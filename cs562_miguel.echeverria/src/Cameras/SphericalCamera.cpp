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
#include "Platform/InputMgr.h"
#include "Platform/FrameRateController.h"
#include "Math/Interpolation/InterpolationFunctions.h"


namespace cs460
{
	// Update the view matrix with the current parameters
	void SphericalCamera::update_view_mtx()
	{
		const glm::vec3& upVec = get_up_vec();
		if (glm::epsilonEqual(glm::length(upVec), 0.0f, FLT_EPSILON))
			return;

		const glm::vec3& realFocalPoint = get_real_focal_point();
		const glm::vec3& viewVec = glm::normalize(realFocalPoint - m_position);

		m_viewMtx = glm::lookAt(m_position, realFocalPoint, upVec);
	}


	// Getters for the camera basis vectors
	glm::vec3 SphericalCamera::get_view_vec() const
	{
		// If length of (get_real_focal_point() - pos) is 0, can't normalize safely
		const glm::vec3& viewVec = get_real_focal_point() - m_position;
		if (glm::epsilonEqual(glm::length(viewVec), 0.0f, FLT_EPSILON))
			return viewVec;

		return glm::normalize(viewVec);
	}
	glm::vec3 SphericalCamera::get_right_vec() const
	{
		// If length of (get_real_focal_point() - pos) is 0, can't cross and normalize safely
		const glm::vec3& viewVec = get_real_focal_point() - m_position;
		if (glm::epsilonEqual(glm::length(viewVec), 0.0f, FLT_EPSILON))
			return glm::vec3(0.0f, 0.0f, 0.0f);

		// If viewVec and globalUp are colinear (their cross will
		// give 0 vector), then use a different globalUp
		const glm::vec3& globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3& rightVec = glm::cross(viewVec, globalUp);

		if (glm::epsilonNotEqual(glm::length(rightVec), 0.0f, FLT_EPSILON))
			return glm::normalize(rightVec);

		const glm::vec3& globalUp2 = glm::vec3(0.0f, 0.0f, 1.0f);
		const glm::vec3& rightVec2 = glm::cross(viewVec, globalUp2);
		return glm::normalize(rightVec2);
	}
	glm::vec3 SphericalCamera::get_up_vec() const
	{
		// Get the right vector and do a sanity check
		const glm::vec3& rightVec = get_right_vec();
		if (glm::epsilonEqual(glm::length(rightVec), 0.0f, FLT_EPSILON))
			return glm::vec3(0.0f, 0.0f, 0.0f);

		// The view vec is already safe
		const glm::vec3& viewVec = get_real_focal_point() - m_position;

		return glm::normalize(glm::cross(rightVec, viewVec));
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

	glm::vec3 SphericalCamera::get_focal_offset() const
	{
		return m_focalOffset;
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

	void SphericalCamera::set_focal_offset(const glm::vec3& offset)
	{
		m_focalOffset = offset;
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
		InputMgr& inputMgr = InputMgr::get_instance();
		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();

		const glm::vec2 rightStick2d = inputMgr.get_gamepad_stick_vec(GAMEPAD::right_stick);
		//float rightStickLenth = glm::length(rightStick2d);
		float rightStickAngle = glm::degrees(atan2(rightStick2d.y, rightStick2d.x));

		const float MAX_Y_ANGLE_SPEED = 150.0f;
		const float MAX_POLAR_ANGLE_SPEED = 150.0f;

		// Update the y angle
		if (glm::abs(rightStick2d.x) > 0.2f)
		{
			float prevYAngle = m_yAngle;
			float targetYAngle = m_yAngle + (rightStick2d.x * MAX_Y_ANGLE_SPEED * dt);
			m_yAngle = lerp(prevYAngle, targetYAngle, dt * 50.0f);
		}
		// Update the polar angle
		if (glm::abs(rightStick2d.y) > 0.4f)
		{
			float prevPolarAngle = m_polarAngle;
			float targetAngle = prevPolarAngle + (rightStick2d.y * MAX_POLAR_ANGLE_SPEED * dt);
			m_polarAngle = lerp(prevPolarAngle, targetAngle, dt * 50.0f);

			// Upper limit
			if ((prevPolarAngle > 5.0f && m_polarAngle < 5.0f) || (prevPolarAngle < -5.0f && m_polarAngle > -5.0f))
				m_polarAngle = prevPolarAngle;

			// Lower limit
			if ((prevPolarAngle < 175.0f && m_polarAngle > 175.0f) || (prevPolarAngle > -175.0f && m_polarAngle < -175.0f))
				m_polarAngle = prevPolarAngle;
		}

		update_position();
	}


	// Returns m_focalPoint if m_focalNode is null, and the world position of m_focalNode otherwise.
	glm::vec3 SphericalCamera::get_real_focal_point() const
	{
		glm::vec3 realFocalPoint;
		if (m_focalNode)
			realFocalPoint = m_focalNode->m_worldTr.m_position;
		else
			realFocalPoint = m_focalPoint;
		return realFocalPoint + m_focalOffset;
	}
	
	// Update the inherited m_position base on the current spherical coordinates parameters
	void SphericalCamera::update_position()
	{
		const glm::vec3& realFocalPoint = get_real_focal_point();

		float polarAngleRadians = glm::radians(m_polarAngle);
		float yAngleRadians = glm::radians(m_yAngle);

		float x = m_radius * sin(polarAngleRadians) * cos(yAngleRadians);
		float y = m_radius * cos(polarAngleRadians);
		float z = m_radius * sin(polarAngleRadians) * sin(yAngleRadians);

		glm::vec3 dir(x, y, z);

		m_position = realFocalPoint + dir;

		if (std::_Is_nan(m_position.x))
			std::cout << "NAN POSITION.X\n";
	}
}