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
#include "Components/ModelInstance.h"
#include "Math/Interpolation/EasingFunctions.h"
#include "Components/AnimationReference.h"


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


	void PiecewiseCurve::initialize()
	{
		// Create a node with a model whose position will be updated over time to follow the curve

		Scene& scene = Scene::get_instance();
		SceneNode* rootNode = scene.get_root();

		m_nodeToMove = rootNode->create_child("CurveCesiumMan");
		ModelInstance* modelInst = m_nodeToMove->add_component<ModelInstance>();
		modelInst->change_model("data/Models/rigged figure/CesiumMan.gltf");
		m_nodeToMove->m_localTr.m_scale = glm::vec3(0.5f, 0.5f, 0.5f);
		AnimationReference* animComp = m_nodeToMove->get_component<AnimationReference>();
		animComp->change_animation(0, "");
	}

	void PiecewiseCurve::update()
	{
		// Gather data before pause check because other stuff depends on said data
		gather_children_data(m_timeValues, m_propertyValues);

		// Don't update if paused
		if (m_paused)
			return;

		// Nothing to update if there are less than 2 points, or less than 2 entries in the arc length table
		if (m_timeValues.size() < 2 || m_arcLengthTable.size() < 2)
			return;

		update_position();
		update_orientation();

		check_bounds();
	}

	void PiecewiseCurve::debug_draw()
	{
		// This will call debug_draw_linear when curve is linear
		if (m_drawCurve)
			debug_draw_cubic_spline(m_curveType);

		if (DebugRenderer::s_enableTableSamplesDrawing && m_drawSamples)
			debug_draw_table_samples(m_curveType);

		// Don't draw moving object if specified
		if (!DebugRenderer::s_enableMovingObjectDrawing || !m_drawMovingObject)
			return;

		// Draw the object moving along the curve
		if (m_totalDistance > 0.0f)
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

		if (m_arcLengthTable.size() > 0)
			m_totalDistance = m_arcLengthTable.back().m_arcLength;
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
		if (tableSize <= 1)
			return 0.0f;

		int low = 0;
		int high = (int)tableSize - 1;
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
		float result = 0.0f;

		if (m_useAdaptive)
			result = get_arc_length_from_tn_adaptive(tn);
		else
			result = get_arc_length_from_tn_uniform(tn);

		return result;
	}


	void PiecewiseCurve::change_curve_type(CURVE_TYPE newType)
	{
		if (newType == m_curveType)
			return;

		// Reset variables
		m_curveType = newType;
		//m_pointCount = 0;
		//m_tangentCount = 0;
		//m_controlPointCount = 0;
		m_prevDistance = 0.0f;
		m_distanceTravelled = 0.0f;
		m_direction = 1.0f;

		// TODO: Organize this better
		for (SceneNode* childNode : get_owner()->get_children())
		{
			if (childNode->get_component<CurvePoint>())
			{
				for (SceneNode* childNode2 : childNode->get_children())
				{
					CurveControlPoint* controlPointComp = childNode2->get_component<CurveControlPoint>();
					CurveTangent* tangentComp = childNode2->get_component<CurveTangent>();
					if (newType == CURVE_TYPE::HERMITE && controlPointComp)
					{
						bool wasLeftControlPoint = controlPointComp->get_is_left_control_point();
						childNode2->delete_component<CurveControlPoint>();
						childNode2->add_component<CurveTangent>()->set_is_in_tangent(wasLeftControlPoint);
					}
					else if (newType == CURVE_TYPE::BEZIER && tangentComp)
					{
						bool wasInTangent = tangentComp->get_is_in_tangent();
						childNode2->delete_component<CurveTangent>();
						childNode2->add_component<CurveControlPoint>()->set_is_left_control_point(wasInTangent);
					}
				}
			}
		}

		// Delete all the children curve points and tangents
		//get_owner()->delete_all_children_with_comp<CurvePoint>();
	}

	void PiecewiseCurve::change_finish_mode(FINISH_MODE newMode)
	{
		m_finishMode = newMode;
		m_prevDistance = 0.0f;
		m_distanceTravelled = 0.0f;
		m_direction = 1.0f;
	}


	void PiecewiseCurve::reset_animation()
	{
		m_currentTime = 0.0f;
		m_distanceTravelled = 0.0f;
		m_prevDistance = 0.0f;
	}


	void PiecewiseCurve::on_gui()
	{
		// Curve type selection
		ImGui::Text("CURVE CONFIG");
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

		ImGui::Text("CURVE FOLLOWING");

		// Orientation control
		ImGui::Text("Orientation Control:");
		int orientationMode = m_useFrenetFrame;
		ImGui::RadioButton("Center Of Interest", &orientationMode, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Frenet Frame", &orientationMode, 1);
		m_useFrenetFrame = orientationMode;
		ImGui::Checkbox("Debug Draw Frenet Frame", &m_debugDrawFrenetFrame);

		// Speed control
		ImGui::Text("Speed Control:");
		int speedMode = m_useConstantSpeed;
		if (ImGui::RadioButton("Constant Speed", &speedMode, 1))
			reset_animation();
		ImGui::SameLine();
		if (ImGui::RadioButton("Ease In/Out", &speedMode, 0))
			reset_animation();
		m_useConstantSpeed = speedMode;

		ImGui::SameLine();
		if (ImGui::Checkbox("Sync Anim", &m_syncAnim))
		{
			// Reset timescale to default if not syncing
			if (!m_syncAnim)
				if (m_nodeToMove)
					if (AnimationReference* animComp = m_nodeToMove->get_component<AnimationReference>())
						animComp->set_anim_time_scale(1.0f);
		}

		
		if (m_useConstantSpeed)
		{
			ImGui::SliderFloat("Speed (m/s)", &m_speed, 0.01f, 10.0f, "%.2f");
			ImGui::Text(/*"Unscaled */"Total Duration (s): %.2f", m_totalDistance / m_speed);
		}
		else
		{
			if (ImGui::SliderFloat("Total Duration (s)", &m_totalDuration, 0.01f, 20.0f, "%.2f"))
				reset_animation();

			ImGui::Text(/*"Scaled */"Speed (m/s): %.2f", m_speed);

			if (ImGui::SliderFloat("t1", &m_accelerateEndTime, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp))
			{
				m_accelerateEndTime = m_accelerateEndTime > m_deccelerateStartTime ? m_deccelerateStartTime : m_accelerateEndTime;
				reset_animation();
			}
			if (ImGui::SliderFloat("t2", &m_deccelerateStartTime, m_accelerateEndTime, 1.0f, "%.3f", ImGuiSliderFlags_::ImGuiSliderFlags_AlwaysClamp))
			{
				m_deccelerateStartTime = m_deccelerateStartTime < m_accelerateEndTime ? m_accelerateEndTime : m_deccelerateStartTime;
				reset_animation();
			}
		}
		//if (ImGui::SliderFloat("TimeScale", &m_timeScale, 0.01f, 5.0f, "%.2f"))
		//{
		//	if (!m_useConstantSpeed)
		//		reset_animation();
		//}

		ImGui::Checkbox("Paused", &m_paused);

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

		

		ImGui::Text("Table Type:");
		int buttonVal = m_useAdaptive;
		ImGui::RadioButton("Uniform", &buttonVal, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Adaptive", &buttonVal, 1);
		m_useAdaptive = buttonVal;

		// Can only change number of samples in forward (Minimum of 2 samples per curve)
		if (!m_useAdaptive)
		{
			if (ImGui::SliderInt("Table Samples", &m_numberOfSamples, 2, 128))
				m_numberOfSamples = m_numberOfSamples < 2 ? 2 : m_numberOfSamples;
		}
		else
		{
			if (ImGui::SliderInt("Forced Subdivisions", &m_forcedSubdivisions, 0, 6))
				m_forcedSubdivisions = m_forcedSubdivisions < 0 ? 0 : m_forcedSubdivisions;

			if (ImGui::SliderFloat("Initial Tolerance", &m_initialTolerance, 0.0f, 1.0f))//, 0.005f))
				m_initialTolerance = m_initialTolerance < 0.0f ? 0.0f : m_initialTolerance;
		}


		ImGui::Checkbox("Show Table", &m_showTable);
		ImGui::SameLine();
		ImGui::Checkbox("Draw Samples", &m_drawSamples);

		if (ImGui::Button("Build Table"))
		{
			build_arc_length_table(m_useAdaptive);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Table"))
		{
			clear_arc_length_table();
		}
		

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
			ImGui::TableSetupColumn("Arc Length (m)");
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


	void PiecewiseCurve::check_bounds()
	{
		if (m_useConstantSpeed)
			check_distance_bounds();
		else
			check_timer_bounds();
	}
	
	void PiecewiseCurve::check_distance_bounds()
	{
		// Going forward and reached the end
		if (m_distanceTravelled > m_totalDistance)
		{
			// Clamp the timer
			if (m_finishMode == FINISH_MODE::STOP)
				m_distanceTravelled = m_totalDistance;
			// Restart the timer
			else if (m_finishMode == FINISH_MODE::RESTART)
				m_distanceTravelled = 0.0f;
			// Go in the opposite direction
			else if (m_finishMode == FINISH_MODE::PINPONG)
			{
				m_distanceTravelled = m_totalDistance;
				m_direction = -1.0f;
			}
		}
		// Going backwards and reached the beginning
		else if (m_distanceTravelled < 0.0f)
		{
			m_distanceTravelled = 0.0f;
			m_direction = 1.0f;
		}
	}

	void PiecewiseCurve::check_timer_bounds()
	{
		// Going forward and reached the end
		if (m_currentTime > m_totalDuration)
		{
			// Clamp the timer
			if (m_finishMode == FINISH_MODE::STOP)
			{
				m_currentTime = m_totalDuration;
				m_distanceTravelled = m_totalDistance;
			}
			// Restart the timer
			else if (m_finishMode == FINISH_MODE::RESTART)
			{
				m_currentTime = 0.0f;
				m_distanceTravelled = 0.0f;
				m_prevDistance = 0.0f;
			}
			// Go in the opposite direction
			else if (m_finishMode == FINISH_MODE::PINPONG)
			{
				m_currentTime = m_totalDuration;
				m_distanceTravelled = m_totalDistance;
				m_direction = -1.0f;
			}
		}
		// Going backwards and reached the beginning
		else if (m_currentTime < 0.0f)
		{
			m_currentTime = 0.0f;
			m_distanceTravelled = 0.0f;
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
		float dt = frc.get_dt_float() * 0.5f;	// * 0.5f to add a little bit more precision
		glm::vec3 prevPos{0.0f, 0.0f, 0.0f};
		if (type == CURVE_TYPE::HERMITE)
			prevPos = piecewise_hermite(m_timeValues, m_propertyValues, 0.0f);
		else if (type == CURVE_TYPE::BEZIER)
			prevPos = piecewise_bezier(m_timeValues, m_propertyValues, 0.0f);
		else if (type == CURVE_TYPE::CATMULL_ROM)
			prevPos = piecewise_catmull_rom(m_timeValues, m_propertyValues, 0.0f);


		for (float dist = dt; dist < 1.0f/*m_totalDistance*/; dist += dt)
		{
			// Compute the current point in the spline according to the given type
			glm::vec3 currentPos{ 0.0f, 0.0f, 0.0f };
			
			if (type == CURVE_TYPE::HERMITE)
				currentPos = piecewise_hermite(m_timeValues, m_propertyValues, dist);//get_tn_from_arc_length(dist));
			else if (type == CURVE_TYPE::BEZIER)
				currentPos = piecewise_bezier(m_timeValues, m_propertyValues, dist);//get_tn_from_arc_length(dist));
			else if (type == CURVE_TYPE::CATMULL_ROM)
				currentPos = piecewise_catmull_rom(m_timeValues, m_propertyValues, dist);//get_tn_from_arc_length(dist));

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

	void PiecewiseCurve::debug_draw_table_samples(CURVE_TYPE type)
	{
		for (int i = 0; i < m_arcLengthTable.size(); ++i)
		{
			const glm::vec3& pos = interpolate_position(m_arcLengthTable[i].m_param, type);
			float halfSize = DebugRenderer::s_tableSampleSize * 0.5f;

			AABB box;
			box.m_min = pos - glm::vec3(halfSize, halfSize, halfSize);
			box.m_max = pos + glm::vec3(halfSize, halfSize, halfSize);
			DebugRenderer::draw_aabb(box, DebugRenderer::s_tableSampleColor);
		}
	}


	// Performs binary search on the arc lengths of the table, and returns the lower
	// and upper indices for the interpolation in low and high. Also returns -1 if
	// arcLength was not found in the table, and its index in the table if it was found.
	int PiecewiseCurve::binary_search_arc_length(float arcLength, int& low, int& high)
	{
		// Is arclength the middle arc length?
		// Is arclength < than the middle arc length?
		// Yes: Move high to current middle - 1 and leave low the same
		// No:  Move low to current middle + 1 and leave high the same
		// Repeat until low > high

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

	// Does the same as binary_search_arc_length, but for a parameter. The code is not reused
	// because the way the table data structure is organized doesn't allow me to do so.
	int PiecewiseCurve::binary_search_parameter(float param, int& low, int& high)
	{
		// Refer to binary_search_arc_length for a more detailed algorithm explanation

		if (low <= high)
		{
			int middle = low + (int)glm::floor((high - low) / 2.0f);
			float middleParam = m_arcLengthTable[middle].m_param;

			// Case: we have found the exact element
			if (glm::epsilonEqual(middleParam, param, FLT_EPSILON))
				return middle;

			// Update low or high depending on result of comparison
			if (param < middleParam)
				high = middle - 1;
			else
				low = middle + 1;

			// Recurse to the next region of the array
			return binary_search_parameter(param, low, high);
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
		clear_arc_length_table();

		// The segments to test in the algorithm (each holds the start parameter and end parameters)
		std::list<SubdivisionEntry> segments;
		
		// Initializing the table and segments with the first point and first segment (start to end) respectively
		m_arcLengthTable.push_back({ 0.0f, 0.0f });
		segments.push_back({ 0.0f, 1.0f, 0 });


		while (!segments.empty())
		{
			// Get the start and end parameters of the current segment to evaluate
			SubdivisionEntry currentSegment = segments.front();
			segments.pop_front();

			// Compute the start, end and mid points of the current segment
			const glm::vec3& startPos = interpolate_position(currentSegment.m_start, m_curveType);
			const glm::vec3& endPos = interpolate_position(currentSegment.m_end, m_curveType);
			float midParam = 0.5f * (currentSegment.m_start + currentSegment.m_end);
			const glm::vec3& midPoint = interpolate_position(midParam, m_curveType);

			// Compute the total length
			const glm::vec3& totalSegment = endPos - startPos;
			float totalLength = glm::length(totalSegment);

			// Compute the combined length of each of the half segments
			const glm::vec3& firstSegment = midPoint - startPos;
			float firstLength = glm::length(firstSegment);
			const glm::vec3& secondSegment = endPos - midPoint;
			float secondLength = glm::length(secondSegment);
			float combinedLength = firstLength + secondLength;

			// Compare the two lengths
			float difference = glm::abs(combinedLength - totalLength);

			const float bias = 0.001f;	// Bias to avoid infinite loops if initial tolerance is 0
			float currentTolerance = m_initialTolerance / glm::pow(2.0f, (float)currentSegment.m_subdivisionLevel) + bias;

			// Acceptable error (and forced subdivisions have been completed), add
			// total length as table entry (length to start is already on table)
			if (difference <= currentTolerance && currentSegment.m_subdivisionLevel >= m_forcedSubdivisions)
			{
				ArcLengthTableEntry entry;
				entry.m_param = currentSegment.m_end;
				float prevArcLength = get_arc_length_from_tn_adaptive(currentSegment.m_start);
				entry.m_arcLength = prevArcLength + totalLength;
				m_arcLengthTable.push_back(entry);
			}
			// Not acceptable error, subdivide and perform the algorithm again
			else
			{
				segments.push_front({ midParam, currentSegment.m_end, currentSegment.m_subdivisionLevel + 1 });
				segments.push_front({ currentSegment.m_start, midParam, currentSegment.m_subdivisionLevel + 1 });
			}
		}
	}


	// Since params are spaced out uniformally, the index of tn can be retreived directly
	float PiecewiseCurve::get_arc_length_from_tn_uniform(float tn)
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

	// Since params are not spaced out uniformally, binary search needs to be made
	float PiecewiseCurve::get_arc_length_from_tn_adaptive(float tn)
	{
		size_t tableSize = m_arcLengthTable.size();
		if (tableSize <= 1)
			return 0.0f;

		int low = 0;
		int high = (int)tableSize - 1;
		int paramIdx = binary_search_parameter(tn, low, high);

		// If exact parameter was found, return its arc length from the table
		if (paramIdx >= 0)
			return m_arcLengthTable[paramIdx].m_arcLength;

		// Otherwise, linearly interpolate between the lower and upper arc lengths

		// Handle out of bounds cases
		if (high < 0)
			return 0.0f;
		if (low >= tableSize)
			return m_arcLengthTable.back().m_arcLength;

		// Linear interpolation to get the final arc length
		int startIdx = high < low ? high : low;
		int endIdx = high < low ? low : high;
		float currentTime = tn - m_arcLengthTable[startIdx].m_param;
		float intervalDuration = m_arcLengthTable[endIdx].m_param - m_arcLengthTable[startIdx].m_param;
		float lerpTn = currentTime / intervalDuration;
		float result = glm::mix(m_arcLengthTable[startIdx].m_arcLength, m_arcLengthTable[endIdx].m_arcLength, lerpTn);
		return result;
	}


	// Move the object along the curve either with constant velocity or with a ease in/out distance-time function.
	void PiecewiseCurve::update_position()
	{
		float dt = FrameRateController::get_instance().get_dt_float();
		AnimationReference* animComp = m_nodeToMove->get_component<AnimationReference>();

		// Interpolate the position in the curve (based on curve type and speed mode)
		if (m_useConstantSpeed)
		{
			if (animComp && m_syncAnim)
				animComp->set_anim_time_scale(m_speed / m_referenceSpeed);

			m_currentPos = interpolate_position(get_tn_from_arc_length(m_distanceTravelled), m_curveType);
			m_distanceTravelled += m_speed * dt * /* m_timeScale **/ m_direction;
		}
		else
		{
			m_prevDistance = m_distanceTravelled;

			if (animComp && m_syncAnim)
				animComp->set_anim_time_scale(m_speed / m_referenceSpeed);

			// Update the position of the object
			m_currentPos = interpolate_position(get_tn_from_arc_length(m_distanceTravelled), m_curveType);

			// Compute the current speed
			float normalizedTime = m_currentTime / m_totalDuration;
			float normalizedDist = ease_in_out(normalizedTime, m_accelerateEndTime, m_deccelerateStartTime);
			float currentDist = normalizedDist * m_totalDistance;
			m_speed = glm::abs(currentDist - m_prevDistance) / dt;

			// Update the distance travelled with the current speed and the timer to sample the easing curve
			m_distanceTravelled += m_speed * dt * m_direction;
			m_currentTime += dt * /*m_timeScale **/ m_direction;
		}

		m_nodeToMove->m_localTr.m_position = m_currentPos;
	}

	// Orient the character using a basic frenet frame
	void PiecewiseCurve::update_orientation()
	{
		glm::vec3 firstDerivative{1.0f, 0.0f, 0.0f};
		glm::vec3 secondDerivative{ 0.0f, 0.0f, -1.0f };

		if (m_curveType == CURVE_TYPE::LINEAR)
		{
			firstDerivative = piecewise_lerp(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 1u);
		}
		else if (m_curveType == CURVE_TYPE::HERMITE)
		{
			firstDerivative = piecewise_hermite(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 1u);
			secondDerivative = piecewise_hermite(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 2u);
		}
		else if (m_curveType == CURVE_TYPE::CATMULL_ROM)
		{
			firstDerivative = piecewise_catmull_rom(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 1u);
			secondDerivative = piecewise_catmull_rom(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 2u);
		}
		else
		{
			firstDerivative = piecewise_bezier(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 1u);
			secondDerivative = piecewise_bezier(m_timeValues, m_propertyValues, get_tn_from_arc_length(m_distanceTravelled), 2u);
		}

		const glm::vec3& tangent = m_direction < 0.0f ? -glm::normalize(firstDerivative) : glm::normalize(firstDerivative);
		const glm::vec3& normal = glm::normalize(glm::cross(secondDerivative, tangent));
		const glm::vec3& bitangent = glm::normalize(glm::cross(tangent, normal));


		// Debug draw the basis vectors and second derivative

		if (m_debugDrawFrenetFrame)
		{

			float vecScale = 1.0f;
			// Debug draw tangent
			Segment seg;
			seg.m_start = m_currentPos;
			seg.m_end = (seg.m_start + tangent) * vecScale;
			DebugRenderer::draw_segment(seg, { 1.0f, 0.0f, 0.0f, 1.0f });

			// Debug draw bitangent
			seg.m_start = m_currentPos;
			seg.m_end = (seg.m_start + bitangent) * vecScale;
			DebugRenderer::draw_segment(seg, { 0.0f, 1.0f, 0.0f, 1.0f });

			// Debug draw normal
			seg.m_start = m_currentPos;
			seg.m_end = (seg.m_start + normal) * vecScale;
			DebugRenderer::draw_segment(seg, { 0.0f, 0.0f, 1.0f, 1.0f });

			// Debug draw second derivative
			seg.m_start = m_currentPos;
			seg.m_end = seg.m_start + secondDerivative;
			DebugRenderer::draw_segment(seg, { 1.0f, 1.0f, 0.0f, 1.0f });
		}


		glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
		glm::quat resultOrientation;

		if (m_useFrenetFrame && m_curveType != CURVE_TYPE::LINEAR)
			resultOrientation = glm::quatLookAtLH(tangent, normal);
		else
			resultOrientation = glm::quatLookAtLH(tangent, globalUp);
		m_nodeToMove->m_localTr.m_orientation = resultOrientation;
	}
}