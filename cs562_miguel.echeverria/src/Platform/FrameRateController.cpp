/**
* @file FrameRateController.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Time management singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "FrameRateController.h"
#include <GLFW/glfw3.h>


namespace cs460
{
	FrameRateController::FrameRateController()
		:	m_dt(1.0f / 60.0f)
	{

	}

	FrameRateController::~FrameRateController()
	{

	}

	FrameRateController& FrameRateController::get_instance()
	{
		static FrameRateController instance;
		return instance;
	}


	// Keep track of time ellapsed on each frame
	void FrameRateController::end_frame()
	{
		double currentTime = glfwGetTime();
		m_dt = currentTime - m_lastTime;
		m_lastTime = currentTime;
	}


	// Getters for the delta time of this frame
	float FrameRateController::get_dt_float() const
	{
		return static_cast<float>(m_dt);
	}

	double FrameRateController::get_dt() const
	{
		return m_dt;
	}
}