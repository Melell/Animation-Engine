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

		// Nothing to update if there are less than 2 points
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
		// This will call debug_draw_linear when curve is linear
		if (m_drawCurve)
			debug_draw_cubic_spline(m_curveType);

		// Don't draw moving object if specified
		if (!DebugRenderer::s_enableMovingObjectDrawing || !m_drawMovingObject)
			return;

		// Draw the object moving along the curve
		if (m_totalDuration > 0.0f)
		{
			float halfSize = DebugRenderer::s_movingObjectSize * 0.5f;
			glm::vec3 halfDiagonal(halfSize, halfSize, halfSize);
			AABB box;
			box.m_min = m_currentPos - halfDiagonal;
			box.m_max = m_currentPos + halfDiagonal;
			DebugRenderer::draw_aabb(box, DebugRenderer::s_movingObjectColor);
		}
	}


	// Builds the arc length table using forward differencing, or adaptive forward differencing.
	void PiecewiseCurve::build_arc_length_table(bool useAdaptive)
	{
		if (useAdaptive)
			build_table_adaptive();
		else
			build_table_uniform();
	}

	void PiecewiseCurve::clear_arc_length_table()
	{
		m_arcLengthTable.clear();
	}


	glm::vec3 PiecewiseCurve::interpolate_position(float tn, CURVE_TYPE type)
	{
		// Handle corner cases
		size_t numberOfPoints = m_timeValues.size();
		if (numberOfPoints == 0)
			return glm::vec3(0.0f, 0.0f, 0.0f);

		if (numberOfPoints == 1 && (type == CURVE_TYPE::LINEAR || type == CURVE_TYPE::CATMULL_ROM))
			return glm::make_vec3(m_propertyValues.data());
		else if (numberOfPoints == 1)
			return glm::make_vec3(m_propertyValues.data() + 3);

		// Interpolate the position in the curve (based on curve type)
		if (type == CURVE_TYPE::LINEAR)
			return piecewise_lerp(m_timeValues, m_propertyValues, tn);
		else if (type == CURVE_TYPE::HERMITE)
			return piecewise_hermite(m_timeValues, m_propertyValues, tn);
		else if (type == CURVE_TYPE::CATMULL_ROM)
			return piecewise_catmull_rom(m_timeValues, m_propertyValues, tn);
		else if (type == CURVE_TYPE::BEZIER)
			return piecewise_bezier(m_timeValues, m_propertyValues, tn);

		return m_currentPos;
	}

	float PiecewiseCurve::get_tn_from_arc_length(float arcLength)
	{
		size_t tableSize = m_arcLengthTable.size();
		if (m_arcLengthTable.size() <= 1)
			return 0.0f;

		int low = 0;
		int high = tableSize - 1;
		int arcLengthIdx = binary_search_arc_length(arcLength, low, high);
		
		// If exact arclength was found, return its parameter from the table
		if (arcLengthIdx >= 0)
			return m_arcLengthTable[arcLengthIdx].m_param;

		// Otherwise, linearly interpolate between the lower and upper parameters

		// Handle out of bounds cases
		if (high < 0)
			return 0.0f;
		if (low >= tableSize)
			return m_arcLengthTable.back().m_param;

		// Linear interpolation to get the final parameter
		int startIdx = high < low ? high : low;
		int endIdx = high < low ? low : high;
		float currentArcLength = arcLength - m_arcLengthTable[startIdx].m_arcLength;
		float intervalArcLength = m_arcLengthTable[endIdx].m_arcLength - m_arcLengthTable[startIdx].m_arcLength;
		float tn = currentArcLength / intervalArcLength;
		return glm::mix(m_arcLengthTable[startIdx].m_param, m_arcLengthTable[endIdx].m_param, tn);
	}

	float PiecewiseCurve::get_arc_length_from_tn(float tn)
	{
		size_t tableSize = m_arcLengthTable.size();
		if (tableSize <= 1)
			return 0.0f;

		// Clamp just in case it wasn't already in range [0, 1]
		float realTn = glm::clamp(tn, 0.0f, 1.0f);

		// Compute the timestep between each element
		float timeStep = 1.0f / (tableSize - 1);

		// Calculate the lower and upper table indices that corresponds to realTn
		unsigned lowerTableIdx = (unsigned)glm::floor(realTn / timeStep);
		unsigned upperTableIdx = lowerTableIdx + 1;

		// Corner case: We are exactly in the last point
		if (upperTableIdx >= tableSize)
			return m_arcLengthTable[lowerTableIdx].m_arcLength;

		// Otherwise, do linear interpolation to get a more accurate arc length
		float minTime = m_arcLengthTable[lowerTableIdx].m_param;
		float maxTime = m_arcLengthTable[upperTableIdx].m_param;
		float intervalDuration = maxTime - minTime;
		float currentTime = tn - minTime;
		float lerpTn = currentTime / intervalDuration;

		float minArcLength = m_arcLengthTable[lowerTableIdx].m_arcLength;
		float maxArcLength = m_arcLengthTable[upperTableIdx].m_arcLength;

		return glm::mix(minArcLength, maxArcLength, lerpTn);
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

		ImGui::Checkbox("Draw Moving Object", &m_drawMovingObject);
		ImGui::SameLine();
		ImGui::Checkbox("Draw Curve", &m_drawCurve);

		ImGui::Separator();

		ImGui::Text("ARC-LENGTH TABLE");

		// Minimum of 2 samples per curve
		if (ImGui::DragInt("Table Samples", &m_numberOfSamples))
		{
			m_numberOfSamples = m_numberOfSamples < 2 ? 2 : m_numberOfSamples;
		}

		ImGui::Text("Table Type:");
		int buttonVal = m_useAdaptive;
		ImGui::RadioButton("Uniform", &buttonVal, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Adaptive", &buttonVal, 1);
		m_useAdaptive = buttonVal;

		if (ImGui::Button("Build Table"))
		{
			build_arc_length_table(m_useAdaptive);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Table"))
		{
			clear_arc_length_table();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Show Table", &m_showTable);

		// Display the table
		arc_length_table_on_gui();
	}

	void PiecewiseCurve::arc_length_table_on_gui()
	{
		ImGuiTableFlags tableFlags = ImGuiTableFlags_::ImGuiTableFlags_RowBg |
									 ImGuiTableFlags_::ImGuiTableFlags_Borders |
									 ImGuiTableFlags_::ImGuiTableFlags_Resizable |
									 ImGuiTableFlags_::ImGuiTableFlags_SizingFixedFit |
									 ImGuiTableFlags_::ImGuiTableFlags_NoHostExtendX |
									 ImGuiTableFlags_::ImGuiTableFlags_Reorderable/* |
									 ImGuiTableFlags_::ImGuiTableFlags_ScrollY |
									 ImGuiTableFlags_::ImGuiTableFlags_ScrollX*/;

		
		float x = 0.0f;
		float y = 0.0f;//ImGui::GetTextLineHeightWithSpacing() * 8.0f;
		if (ImGui::BeginTable("Arc Length Table", 3, tableFlags, ImVec2(x, y)))
		{
			ImGui::TableSetupColumn("Index");
			ImGui::TableSetupColumn("Parameter");
			ImGui::TableSetupColumn("Arc Length");
			ImGui::TableHeadersRow();

			if (m_showTable)
			{
				for (int i = 0; i < m_arcLengthTable.size(); ++i)
				{
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%i", i);

					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%f", m_arcLengthTable[i].m_param);

					ImGui::TableSetColumnIndex(2);
					ImGui::Text("%f", m_arcLengthTable[i].m_arcLength);
				}
			}

			ImGui::EndTable();
		}
	}


	// Get the vectors of time, and their respective values
	void PiecewiseCurve::gather_children_data(std::vector<float>& times, std::vector<float>& values)
	{
		times.clear();
		values.clear();


		// Count the number of points and create the properties vector
		m_pointCount = 0;
		for (SceneNode* child : get_owner()->get_children())
		{
			CurvePoint* pointComp = child->get_component<CurvePoint>();
			if (pointComp == nullptr)
				continue;


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


			// Update the number of points
			++m_pointCount;
		}


		// Nothing else to do if curve is empty
		if (m_pointCount == 0)
			return;

		
		// Add the first time manually, and check if there are more points
		times.push_back(0.0f);
		if (m_pointCount == 1)
			return;

		// Create the times for the rest of points
		float timeStep = 1.0f / (m_pointCount - 1);
		float currTime = timeStep;
		for (unsigned i = 1; i < m_pointCount; ++i)
		{
			// Clamp it just in case
			if (currTime > 1.0f)
				currTime = 1.0f;

			times.push_back(currTime);
			currTime += timeStep;
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
		if (!DebugRenderer::s_enableCurveDrawing)
			return;

		glm::vec3 prevPos(0.0f, 0.0f, 0.0f);
		bool first = true;
		for (SceneNode* child : get_owner()->get_children())
		{
			CurvePoint* pointComp = child->get_component<CurvePoint>();
			if (pointComp == nullptr)
				continue;

			const glm::vec3& currPos = child->m_worldTr.m_position;

			// Draw the point as an aabb
			DebugRenderer::draw_curve_node(currPos, DebugRenderer::s_curvePointColor, DebugRenderer::s_curvePointSize);

			// Draw the line between the previous point and this one (not on the first one)
			if (!first)
			{
				Segment seg;
				seg.m_start = prevPos;
				seg.m_end = currPos;
				DebugRenderer::draw_segment(seg, DebugRenderer::s_curveColor);
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
			if (DebugRenderer::s_enableCurveDrawing)
				DebugRenderer::draw_curve_node(child->m_worldTr.m_position, DebugRenderer::s_curvePointColor, DebugRenderer::s_curvePointSize);

			// Debug draw the tangents/control_points of the current point
			if (DebugRenderer::s_enableTangentDrawing && (type == CURVE_TYPE::HERMITE || type == CURVE_TYPE::BEZIER))
				debug_draw_tangents(child);
		}


		// Don't draw the curve if there is zero, or one point
		if (m_timeValues.size() < 2 || !DebugRenderer::s_enableCurveDrawing)
			return;


		// Draw the actual curve (a line per each timestep)
		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();
		glm::vec3 prevPos{0.0f, 0.0f, 0.0f};
		if (type == CURVE_TYPE::HERMITE)
			prevPos = piecewise_hermite(m_timeValues, m_propertyValues, 0.0f);
		else if (type == CURVE_TYPE::BEZIER)
			prevPos = piecewise_bezier(m_timeValues, m_propertyValues, 0.0f);
		else if (type == CURVE_TYPE::CATMULL_ROM)
			prevPos = piecewise_catmull_rom(m_timeValues, m_propertyValues, 0.0f);

		for (float timer = dt; timer < m_totalDuration; timer += dt)
		{
			// Compute the current point in the spline according to the given type
			glm::vec3 currentPos{ 0.0f, 0.0f, 0.0f };
			
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
			DebugRenderer::draw_segment(seg, DebugRenderer::s_curveColor);

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
			DebugRenderer::draw_curve_node(child->m_worldTr.m_position, DebugRenderer::s_tangentEndpointColor, DebugRenderer::s_tangentEndpointSize);


			// Draw the line from the point to the tangent endpoint
			Segment seg;
			seg.m_start = pointNode->m_worldTr.m_position;
			seg.m_end = child->m_worldTr.m_position;
			DebugRenderer::draw_segment(seg, DebugRenderer::s_tangentLineColor);
		}
	}


	// Performs binary search on the arc lengths of the table, and returns the lower
	// and upper indices for the interpolation in low and high. Also returns -1 if
	// arcLength was not found in the table, and its index in the table if it was found.
	int PiecewiseCurve::binary_search_arc_length(float arcLength, int& low, int& high)
	{
		// Is arclength > than the middle arc length?
		// Yes: Move low to current middle + 1 and leave high the same
		// No:  Move high to current middle - 1 and leave low the same
		// Repeat until number of difference of elements between current first and last <= 1

		if (low <= high)
		{
			int middle = low + (int)glm::floor((high - low) / 2.0f);
			float middleArcLength = m_arcLengthTable[middle].m_arcLength;

			// Case: we have found the exact element
			if (glm::epsilonEqual(middleArcLength, arcLength, FLT_EPSILON))
				return middle;

			// Update low or high depending on result of comparison
			if (arcLength < middleArcLength)
				high = middle - 1;
			else
				low = middle + 1;

			// Recurse to the next region of the array
			return binary_search_arc_length(arcLength, low, high);
		}

		// Exact element not found
		return -1;
	}


	// Helper functions to build the arc length table using uniform forward differencing
	void PiecewiseCurve::build_table_uniform()
	{
		m_arcLengthTable.resize(m_numberOfSamples);

		// Manully set the first element
		m_arcLengthTable[0].m_param = 0.0f;
		m_arcLengthTable[0].m_arcLength = 0.0f;

		// Fill the rest of the table
		float timeStep = 1.0f / (m_numberOfSamples - 1);
		float tn = timeStep;
		for (int i = 1; i < m_numberOfSamples; ++i)
		{
			float accumulatedArcLength = m_arcLengthTable[i - 1].m_arcLength;
			const glm::vec3& prevPos = interpolate_position(tn - timeStep, m_curveType);
			const glm::vec3& currPos = interpolate_position(tn, m_curveType);
			float currArcLength = glm::length(currPos - prevPos);
			m_arcLengthTable[i].m_arcLength = accumulatedArcLength + currArcLength;
			m_arcLengthTable[i].m_param = tn;
			tn += timeStep;
		}
	}

	// Helper functions to build the arc length table using adaptive forward differencing
	void PiecewiseCurve::build_table_adaptive()
	{

	}
}