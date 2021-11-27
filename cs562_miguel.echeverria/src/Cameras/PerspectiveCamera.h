/**
* @file PerspectiveCamera.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the necessary data to form a perspective projection matrix.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "ICamera.h"


namespace cs460
{
	class PerspectiveCamera : public ICamera
	{
	public:

		PerspectiveCamera();
		PerspectiveCamera(float yFov, float aspectRatio, float near, float far);

		// Meant to be implemented by child classes(editor cam, spherical cam etc)
		virtual void update_view_mtx() = 0;

		void update_projection_mtx() override;

		// Getters for the camera parameters
		float get_y_fov() const;
		float get_aspect_ratio() const;
		float get_near() const;
		float get_far() const;

		// Setters for the camera parameters
		void set_y_fov(float yFov);
		void set_aspect_ratio(float aspectRatio);
		void set_near(float near);
		void set_far(float far);

	protected:
		// Meant to be overriden by specific cameras
		virtual void camera_logic() = 0;

	private:
		float m_yFov;
		float m_aspectRatio;
		float m_near;
		float m_far;
	};
}