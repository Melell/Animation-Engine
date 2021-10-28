/**
* @file PiecewiseCurve.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component for creating piecewise curves.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IComponent.h"


namespace cs460
{
	enum class CURVE_TYPE
	{
		LINEAR = 0,
		HERMITE = 1,
		CATMULL_ROM = 2,
		BEZIER = 3
	};



	class PiecewiseCurve : public IComponent
	{
	public:

		PiecewiseCurve();
		virtual ~PiecewiseCurve();

		void update();
		void debug_draw();

		enum class FINISH_MODE
		{
			STOP,
			RESTART,
			PINPONG
		};

		void change_finish_mode(FINISH_MODE newMode);

	private:
		glm::vec3 m_currentPos{0.0f, 0.0f, 0.0f};
		CURVE_TYPE m_curveType = CURVE_TYPE::LINEAR;	// 0 = linear. 1 = hermite. 2 = catmull-rom. 3 = bezier
		unsigned m_pointCount = 0;
		float m_timer = 0.0f;
		float m_totalDuration = 0.0f;
		float m_timeScale = 1.0f;
		float m_direction = 1.0f;
		FINISH_MODE m_finishMode = FINISH_MODE::RESTART;
		bool m_paused = false;

		void on_gui() override;

		void gather_children_data(std::vector<float>& times, std::vector<float>& values);	// Get the vectors of time, and their respective values
		void check_timer_bounds();
		void debug_draw_linear();
	};
}