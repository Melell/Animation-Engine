/**
* @file PerspectiveCamera.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the necessary data to form a perspective projection matrix.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class PerspectiveCamera
	{
	public:

		PerspectiveCamera();
		PerspectiveCamera(float yFov, float aspectRatio, float near, float far);

		const glm::mat4& get_perspective_mtx() const;

	private:
		float m_yFov;
		float m_aspectRatio;
		float m_near;
		float m_far;
		glm::mat4 m_perspectiveMtx;
	};
}