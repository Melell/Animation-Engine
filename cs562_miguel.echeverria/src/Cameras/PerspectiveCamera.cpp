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
		m_perspectiveMtx = glm::perspective(m_yFov, m_aspectRatio, m_near, m_far);
	}

	PerspectiveCamera::PerspectiveCamera(float yFov, float aspectRatio, float near, float far)
		:	m_yFov(yFov),
			m_aspectRatio(aspectRatio),
			m_near(near),
			m_far(far)
	{
		m_perspectiveMtx = glm::perspective(m_yFov, m_aspectRatio, m_near, m_far);
	}

	const glm::mat4& PerspectiveCamera::get_perspective_mtx() const
	{
		return m_perspectiveMtx;
	}
}