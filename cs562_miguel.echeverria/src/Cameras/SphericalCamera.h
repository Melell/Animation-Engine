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

		// Getters for the camera controlling parameters
		SceneNode* get_focal_node() const;
		glm::vec3 get_focal_point() const;
		float get_radius() const;
		float get_y_angle() const;
		float get_polar_angle() const;

		// Setters for the camera controlling parameters
		void set_focal_node(SceneNode* node);
		void set_focal_point(const glm::vec3& point);
		void set_radius(float radius);
		void set_y_angle(float yAngle);
		void set_polar_angle(float polarAngle);

	protected:
		// Actual logic for the spherical camera goes here
		void camera_logic() override;

	private:
		SceneNode* m_focalNode = nullptr;
		glm::vec3 m_focalPoint{ 0.0f, 0.0f, 0.0f };
		float m_radius = 25.0f;
		float m_yAngle = -90.0f;
		float m_polarAngle = 40.0f;
	};
}