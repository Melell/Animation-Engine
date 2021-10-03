/**
* @file FrameRateController.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Time management singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	// Extremely basic at the moment, need to improve it. Also, it is using glfw to get the time.
	class FrameRateController
	{
	public:

		~FrameRateController();
		static FrameRateController& get_instance();

		// Keep track of time ellapsed on each frame
		void end_frame();

		// Getters for the delta time of this frame
		float get_dt_float() const;
		double get_dt() const;

	private:

		double m_dt;
		double m_lastTime;

		FrameRateController();
		FrameRateController(const FrameRateController&) = delete;
		FrameRateController& operator=(const FrameRateController&) = delete;
	};
}