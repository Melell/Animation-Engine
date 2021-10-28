/**
* @file PiecewiseCurve.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component for creating piecewise curves.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "PiecewiseCurve.h"
#include "Composition/SceneNode.h"
#include "CurvePoint.h"
#include "Animation/PiecewiseCurveMgr.h"
#include "Platform/FrameRateController.h"
#include "Math/Interpolation/InterpolationFunctions.h"
#include "Graphics/Systems/DebugRenderer.h"
#include "Math/Geometry/Geometry.h"
#include <imgui/imgui.h>


namespace cs460
{
	PiecewiseCurve::PiecewiseCurve()
	{
		PiecewiseCurveMgr::get_instance().add_piecewise_curve(this);
	}

	PiecewiseCurve::~PiecewiseCurve()
	{
		PiecewiseCurveMgr::get_instance().remove_piecewise_curve(this);
	}


	void PiecewiseCurve::update()
	{
		if (m_paused)
			return;

		std::vector<float> times;
		std::vector<float> values;
		gather_children_data(times, values);

		// Nothing to update unless there are at least 2 points
		if (times.size() < 2)
			return;

		if (m_curveType == CURVE_TYPE::LINEAR)
			m_currentPos = piecewise_lerp(times, values, m_timer);

		m_timer += FrameRateController::get_instance().get_dt_float() * m_timeScale * m_direction;

		check_timer_bounds();
	}

	void PiecewiseCurve::debug_draw()
	{
		if (m_curveType == CURVE_TYPE::LINEAR)
			debug_draw_linear();

		if (m_totalDuration > 0.0f)
		{
			// Draw the object moving along the curve
			glm::vec3 halfDiagonal(0.2f, 0.2f, 0.2f);
			AABB box;
			box.m_min = m_currentPos - halfDiagonal;
			box.m_max = m_currentPos + halfDiagonal;
			DebugRenderer::draw_aabb(box, { 0.1f, 0.9f, 0.4f, 1.0f });
		}
	}


	void PiecewiseCurve::change_finish_mode(FINISH_MODE newMode)
	{
		m_finishMode = newMode;
		m_timer = 0.0f;
		m_direction = 1.0f;
	}


	void PiecewiseCurve::on_gui()
	{
		// Curve type selection
		ImGui::Text("Curve Type:");
		int curveType = (int)m_curveType;
		ImGui::RadioButton("Linear", &curveType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Hermite", &curveType, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Catmull-Rom", &curveType, 2);
		ImGui::SameLine();
		ImGui::RadioButton("Bezier", &curveType, 3);
		m_curveType = (CURVE_TYPE)curveType;

		ImGui::Text("The points/tangents are given as children of this scene node");
		if (ImGui::Button("Add Point"))
		{
			SceneNode* childNode = get_owner()->create_child("Point" + std::to_string(m_pointCount));
			++m_pointCount;
			childNode->add_component<CurvePoint>();
		}

		ImGui::Separator();

		ImGui::Checkbox("Paused", &m_paused);

		ImGui::SliderFloat("TimeScale", &m_timeScale, 0.1f, 5.0f, "%.2f");

		ImGui::Text("Finish Mode:");
		int finishMode = (int)m_finishMode;
		ImGui::RadioButton("Stop", &finishMode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Restart", &finishMode, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Ping-Pong", &finishMode, 2);
		m_finishMode = (FINISH_MODE)finishMode;
	}


	// Get the vectors of time, and their respective values
	void PiecewiseCurve::gather_children_data(std::vector<float>& times, std::vector<float>& values)
	{
		m_totalDuration = 0.0f;
		for (SceneNode* child : get_owner()->get_children())
		{
			CurvePoint* pointComp = child->get_component<CurvePoint>();
			if (pointComp == nullptr)
				continue;
			
			// Get the time
			float time = pointComp->get_time();
			times.push_back(time);

			// Get the position value
			float* pos = glm::value_ptr(child->m_worldTr.m_position);
			values.push_back(pos[0]);
			values.push_back(pos[1]);
			values.push_back(pos[2]);


			// Update the total duration
			m_totalDuration = time;
		}
	}


	void PiecewiseCurve::check_timer_bounds()
	{
		if (m_timer > m_totalDuration)
		{
			if (m_finishMode == FINISH_MODE::STOP)
				m_timer = m_totalDuration;
			else if (m_finishMode == FINISH_MODE::RESTART)
				m_timer = 0.0f;
			else if (m_finishMode == FINISH_MODE::PINPONG)
			{
				m_timer = m_totalDuration;
				m_direction = -1.0f;
			}
		}
		else if (m_timer < 0.0f)
		{
			m_timer = 0.0f;
			m_direction = 1.0f;
		}
	}


	void PiecewiseCurve::debug_draw_linear()
	{
		glm::vec3 prevPos(0.0f, 0.0f, 0.0f);
		bool first = true;
		for (SceneNode* child : get_owner()->get_children())
		{
			CurvePoint* pointComp = child->get_component<CurvePoint>();
			if (pointComp == nullptr)
				continue;

			// Get the position of the current point
			glm::vec3 currPos = child->m_worldTr.m_position;

			AABB box;
			glm::vec3 halfDiagonal(0.1f, 0.1f, 0.1f);
			box.m_min = currPos - halfDiagonal;
			box.m_max = currPos + halfDiagonal;

			// Draw the point as an aabb
			DebugRenderer::draw_aabb(box, { 1.0f, 0.4f, 0.0f, 1.0f });

			// Draw the line between the previous point and this one (not on the first one)
			if (!first)
			{
				Segment seg;
				seg.m_start = prevPos;
				seg.m_end = currPos;
				DebugRenderer::draw_segment(seg, { 1.0f, 1.0f, 1.0f, 1.0f });
			}

			// Update the previous position
			prevPos = currPos;
			first = false;
		}
	}
}