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

		void change_curve_type(CURVE_TYPE newType);
		void change_finish_mode(FINISH_MODE newMode);

	private:
		// Animation control variables
		glm::vec3 m_currentPos{0.0f, 0.0f, 0.0f};
		CURVE_TYPE m_curveType = CURVE_TYPE::LINEAR;
		unsigned m_pointCount = 0;
		unsigned m_tangentCount = 0;
		float m_timer = 0.0f;
		float m_totalDuration = 0.0f;
		float m_timeScale = 1.0f;
		float m_direction = 1.0f;
		FINISH_MODE m_finishMode = FINISH_MODE::RESTART;
		bool m_paused = false;

		// Time-value vectors (the point data, updated every frame from the children)
		std::vector<float> m_timeValues;
		std::vector<float> m_propertyValues;

		glm::vec4 m_pointColor{ 1.0f, 0.4f, 0.0f, 1.0f };		// Orange-ish
		glm::vec4 m_curveColor{ 1.0f, 1.0f, 1.0f, 1.0f };		// White
		glm::vec4 m_tangEndpointColor{0.0f, 1.0f, 0.0f, 1.0f};	// Equivalent to control point in bezier (green)
		glm::vec4 m_tangLineColor{1.0f, 0.0f, 0.0f, 1.0f};		// Equivalent to the vector from the point to the control point in bezier (white)


		void on_gui() override;

		void gather_children_data(std::vector<float>& times, std::vector<float>& values);		// Get the vectors of time, and their respective values
		void search_for_tangents(glm::vec3& inTangent, glm::vec3& outTangent, SceneNode* pointNode);	// Return in inTangent and outTangent the tangent data for pointNode (assuming hermite or bezier)
		void check_timer_bounds();

		void debug_draw_linear();
		void debug_draw_hermite();
		void debug_draw_tangents(SceneNode* pointNode);
	};
}