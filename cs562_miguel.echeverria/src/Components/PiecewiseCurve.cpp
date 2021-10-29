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
#include "Components/CurveTangent.h"
#include "Components/CurveControlPoint.h"
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
		// Gather data before pause check because other stuff depends on said data
		gather_children_data(m_timeValues, m_propertyValues);

		// Don't update if paused
		if (m_paused)
			return;

		// Nothing to update unless there are at least 2 points
		if (m_timeValues.size() < 2)
			return;

		// Interpolate the position in the curve (based on curve type)
		m_currentPos = interpolate_position(m_timer, m_curveType);

		// Update the timer of the animation
		m_timer += FrameRateController::get_instance().get_dt_float() * m_timeScale * m_direction;

		check_timer_bounds();
	}

	void PiecewiseCurve::debug_draw()
	{
		if (m_curveType == CURVE_TYPE::LINEAR)
			debug_draw_linear();
		else if (m_curveType == CURVE_TYPE::HERMITE || m_curveType == CURVE_TYPE::BEZIER || m_curveType == CURVE_TYPE::CATMULL_ROM)
			debug_draw_cubic_spline(m_curveType);


		// Draw the object moving along the curve
		if (m_totalDuration > 0.0f)
		{
			glm::vec3 halfDiagonal(0.2f, 0.2f, 0.2f);
			AABB box;
			box.m_min = m_currentPos - halfDiagonal;
			box.m_max = m_currentPos + halfDiagonal;
			DebugRenderer::draw_aabb(box, { 0.1f, 0.9f, 0.4f, 1.0f });
		}
	}


	glm::vec3 PiecewiseCurve::interpolate_position(float time, CURVE_TYPE type)
	{
		// Interpolate the position in the curve (based on curve type)
		if (type == CURVE_TYPE::LINEAR)
			return piecewise_lerp(m_timeValues, m_propertyValues, time);
		else if (type == CURVE_TYPE::HERMITE)
			return piecewise_hermite(m_timeValues, m_propertyValues, time);
		else if (type == CURVE_TYPE::CATMULL_ROM)
			return piecewise_catmull_rom(m_timeValues, m_propertyValues, time);
		else if (type == CURVE_TYPE::BEZIER)
			return piecewise_bezier(m_timeValues, m_propertyValues, time);

		return m_currentPos;
	}


	void PiecewiseCurve::change_curve_type(CURVE_TYPE newType)
	{
		if (newType == m_curveType)
			return;

		// Reset variables
		m_curveType = newType;
		m_pointCount = 0;
		m_tangentCount = 0;
		m_controlPointCount = 0;
		m_timer = 0.0f;
		m_direction = 1.0f;

		// Delete all the children curve points and tangents
		get_owner()->delete_all_children_with_comp<CurvePoint>();
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
		change_curve_type((CURVE_TYPE)curveType);


		// Adding points and possibly tangents/control points
		ImGui::Text("The points/tangents are given as children of this scene node");
		if (ImGui::Button("Add Point"))
		{
			SceneNode* childNode = get_owner()->create_child("Point" + std::to_string(m_pointCount));
			childNode->add_component<CurvePoint>();
			++m_pointCount;

			// Add the tangents as children of the point (if hermite)
			if (m_curveType == CURVE_TYPE::HERMITE)
			{
				SceneNode* inTanNode = childNode->create_child("Tangent" + std::to_string(m_tangentCount));
				CurveTangent* inTangent = inTanNode->add_component<CurveTangent>();
				inTangent->set_is_in_tangent(true);
				++m_tangentCount;
				
				SceneNode* outTanNode = childNode->create_child("Tangent" + std::to_string(m_tangentCount));
				CurveTangent* outTangent = outTanNode->add_component<CurveTangent>();
				outTangent->set_is_in_tangent(false);
				++m_tangentCount;
			}
			// Also add the control points as children of the point (if bezier)
			else if (m_curveType == CURVE_TYPE::BEZIER)
			{
				SceneNode* inTanNode = childNode->create_child("Control_Point" + std::to_string(m_controlPointCount));
				CurveControlPoint* leftControlPoint = inTanNode->add_component<CurveControlPoint>();
				leftControlPoint->set_is_left_control_point(true);
				++m_controlPointCount;

				SceneNode* outTanNode = childNode->create_child("Control_Point" + std::to_string(m_controlPointCount));
				CurveControlPoint* rightControlPoint = outTanNode->add_component<CurveControlPoint>();
				rightControlPoint->set_is_left_control_point(false);
				++m_controlPointCount;
			}
		}

		ImGui::Separator();

		ImGui::Checkbox("Paused", &m_paused);

		ImGui::SliderFloat("TimeScale", &m_timeScale, 0.1f, 5.0f, "%.2f");

		// Finishing mode selection
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
		times.clear();
		values.clear();

		m_totalDuration = 0.0f;
		m_pointCount = 0;
		for (SceneNode* child : get_owner()->get_children())
		{
			CurvePoint* pointComp = child->get_component<CurvePoint>();
			if (pointComp == nullptr)
				continue;
			

			// Get the time
			float time = pointComp->get_time();
			times.push_back(time);


			// Get the in and out tangents if we are in hermite or bezier
			glm::vec3 inTangent{};
			glm::vec3 outTangent{};
			if (m_curveType == CURVE_TYPE::HERMITE)
				search_for_tangents(inTangent, outTangent, child);
			else if (m_curveType == CURVE_TYPE::BEZIER)
				search_for_control_points(inTangent, outTangent, child);


			// Add the in-tangents data
			if (m_curveType == CURVE_TYPE::HERMITE || m_curveType == CURVE_TYPE::BEZIER)
			{
				values.push_back(inTangent.x);
				values.push_back(inTangent.y);
				values.push_back(inTangent.z);
			}

			// Get the position value
			float* pos = glm::value_ptr(child->m_worldTr.m_position);
			values.push_back(pos[0]);
			values.push_back(pos[1]);
			values.push_back(pos[2]);

			// Add the out-tangents data
			if (m_curveType == CURVE_TYPE::HERMITE || m_curveType == CURVE_TYPE::BEZIER)
			{
				values.push_back(outTangent.x);
				values.push_back(outTangent.y);
				values.push_back(outTangent.z);
			}


			// Update the total duration and the number of points
			m_totalDuration = time;
			++m_pointCount;
		}
	}


	// Return in inTangent and outTangent the tangent data for pointNode (assuming hermite or bezier)
	void PiecewiseCurve::search_for_tangents(glm::vec3& inTangent, glm::vec3& outTangent, SceneNode* pointNode)
	{
		for (SceneNode* child : pointNode->get_children())
		{
			CurveTangent* tangentComp = child->get_component<CurveTangent>();
			if (tangentComp)
			{
				if (tangentComp->get_is_in_tangent())
					inTangent = /*glm::normalize(*/child->m_worldTr.m_position - pointNode->m_worldTr.m_position/*)*/;
				else
					outTangent = /*glm::normalize(*/child->m_worldTr.m_position - pointNode->m_worldTr.m_position/*)*/;
			}
		}
	}

	void PiecewiseCurve::search_for_control_points(glm::vec3& leftControlPoint, glm::vec3& rightControlPoint, SceneNode* pointNode)
	{
		for (SceneNode* child : pointNode->get_children())
		{
			CurveControlPoint* tangentComp = child->get_component<CurveControlPoint>();
			if (tangentComp)
			{
				if (tangentComp->get_is_left_control_point())
					leftControlPoint = child->m_worldTr.m_position;
				else
					rightControlPoint = child->m_worldTr.m_position;
			}
		}
	}


	void PiecewiseCurve::check_timer_bounds()
	{
		// Going forward and reached th end
		if (m_timer > m_totalDuration)
		{
			// Clamp the timer
			if (m_finishMode == FINISH_MODE::STOP)
				m_timer = m_totalDuration;
			// Restart the timer
			else if (m_finishMode == FINISH_MODE::RESTART)
				m_timer = 0.0f;
			// Go in the opposite direction
			else if (m_finishMode == FINISH_MODE::PINPONG)
			{
				m_timer = m_totalDuration;
				m_direction = -1.0f;
			}
		}
		// Going backwards and reached the beginning
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

			const glm::vec3& currPos = child->m_worldTr.m_position;

			// Draw the point as an aabb
			DebugRenderer::draw_curve_node(currPos, m_pointColor);

			// Draw the line between the previous point and this one (not on the first one)
			if (!first)
			{
				Segment seg;
				seg.m_start = prevPos;
				seg.m_end = currPos;
				DebugRenderer::draw_segment(seg, m_curveColor);
			}

			// Update the previous position
			prevPos = currPos;
			first = false;
		}
	}

	void PiecewiseCurve::debug_draw_cubic_spline(CURVE_TYPE type)
	{
		// Not supposed to happen, but just in case
		if (type == CURVE_TYPE::LINEAR)
		{
			debug_draw_linear();
			return;
		}


		// Draw the points and the tangents of the points
		for (SceneNode* child : get_owner()->get_children())
		{
			CurvePoint* pointComp = child->get_component<CurvePoint>();
			if (pointComp == nullptr)
				continue;

			// Draw the point as an aabb
			DebugRenderer::draw_curve_node(child->m_worldTr.m_position, m_pointColor);

			// Debug draw the tangents/control_points of the current point
			if (type == CURVE_TYPE::HERMITE || type == CURVE_TYPE::BEZIER)
				debug_draw_tangents(child);
		}


		// Don't draw the curve if there is zero, or one point
		if (m_timeValues.size() < 2)
			return;


		// Draw the actual curve (a line per each timestep)
		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();
		glm::vec3 prevPos;
		if (type == CURVE_TYPE::HERMITE)
			prevPos = piecewise_hermite(m_timeValues, m_propertyValues, 0.0f);
		else if (type == CURVE_TYPE::BEZIER)
			prevPos = piecewise_bezier(m_timeValues, m_propertyValues, 0.0f);
		else if (type == CURVE_TYPE::CATMULL_ROM)
			prevPos = piecewise_catmull_rom(m_timeValues, m_propertyValues, 0.0f);

		for (float timer = dt; timer < m_totalDuration; timer += dt)
		{
			// Compute the current point in the spline according to the given type
			glm::vec3 currentPos;
			
			if (type == CURVE_TYPE::HERMITE)
				currentPos = piecewise_hermite(m_timeValues, m_propertyValues, timer);
			else if (type == CURVE_TYPE::BEZIER)
				currentPos = piecewise_bezier(m_timeValues, m_propertyValues, timer);
			else if (type == CURVE_TYPE::CATMULL_ROM)
				currentPos = piecewise_catmull_rom(m_timeValues, m_propertyValues, timer);

			// Draw the current line
			Segment seg;
			seg.m_start = prevPos;
			seg.m_end = currentPos;
			DebugRenderer::draw_segment(seg, m_curveColor);

			// Update the previous position
			prevPos = currentPos;
		}
	}

	void PiecewiseCurve::debug_draw_tangents(SceneNode* pointNode)
	{
		for (SceneNode* child : pointNode->get_children())
		{
			if (child->get_component<CurveTangent>() == nullptr && child->get_component<CurveControlPoint>() == nullptr)
				continue;

			// Draw the tangent endpoint
			DebugRenderer::draw_curve_node(child->m_worldTr.m_position, m_tangEndpointColor);


			// Draw the line from the point to the tangent endpoint
			Segment seg;
			seg.m_start = pointNode->m_worldTr.m_position;
			seg.m_end = child->m_worldTr.m_position;
			DebugRenderer::draw_segment(seg, m_tangLineColor);
		}
	}
}