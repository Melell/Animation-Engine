/**
* @file SphericalCamera.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Contains the logic for a spherical camera that orbits around a given focal point.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "PerspectiveCamera.h"


namespace cs460
{
	class SceneNode;


	// Orbits around a given node. If this node is null, it orbits around a given focal point.
	class SphericalCamera : public PerspectiveCamera
	{
	public:

		// Update the view matrix with the current parameters
		void update_view_mtx() override;

		// Getters for the camera basis vectors
		glm::vec3 get_view_vec() const;
		glm::vec3 get_right_vec() const;
		glm::vec3 get_up_vec() const;

		// Getters for the camera controlling parameters
		SceneNode* get_focal_node() const;
		glm::vec3 get_focal_point() const;
		glm::vec3 get_focal_offset() const;
		float get_radius() const;
		float get_y_angle() const;
		float get_polar_angle() const;

		// Setters for the camera controlling parameters
		void set_focal_node(SceneNode* node);
		void set_focal_point(const glm::vec3& point);
		void set_focal_offset(const glm::vec3& offset);
		void set_radius(float radius);
		void set_y_angle(float yAngle);
		void set_polar_angle(float polarAngle);

		bool m_isActive = true;

	protected:
		// Actual logic for the spherical camera goes here
		void camera_logic() override;

	private:
		SceneNode* m_focalNode = nullptr;
		glm::vec3 m_focalPoint{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_focalOffset{ 0.0f, 0.0f, 0.0f };
		float m_radius = 3.5f;
		float m_yAngle = -90.0f;
		float m_polarAngle = 60.0f;


		// Returns m_focalPoint if m_focalNode is null, and the world position of m_focalNode otherwise.
		glm::vec3 get_real_focal_point() const;

		// Update the inherited m_position base on the current spherical coordinates parameters
		void update_position();
	};
}