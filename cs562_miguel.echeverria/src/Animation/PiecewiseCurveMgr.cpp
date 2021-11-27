/**
* @file PiecewiseCurveMgr.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Updates and draws all the piecewise curve components.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "PiecewiseCurveMgr.h"
#include "Components/PiecewiseCurves/PiecewiseCurve.h"
#include "Math/Geometry/Geometry.h"
#include "Math/Geometry/IntersectionTests.h"
#include "Graphics/Systems/DebugRenderer.h"
#include "Composition/SceneNode.h"
#include "Components/PiecewiseCurves/CurvePoint.h"
#include "Components/PiecewiseCurves/CurveTangent.h"
#include "Components/PiecewiseCurves/CurveControlPoint.h"


namespace cs460
{
	PiecewiseCurveMgr& PiecewiseCurveMgr::get_instance()
	{
		static PiecewiseCurveMgr instance;
		return instance;
	}

	PiecewiseCurveMgr::PiecewiseCurveMgr()
	{
	}

	PiecewiseCurveMgr::~PiecewiseCurveMgr()
	{
	}


	// Updates all the piecewise curve components in order
	void PiecewiseCurveMgr::update()
	{
		for (auto curve : m_curves)
			curve->update();
	}

	// Uses debug drawing to render each curve
	void PiecewiseCurveMgr::debug_draw()
	{
		for (auto curve : m_curves)
			curve->debug_draw();
	}


	void PiecewiseCurveMgr::add_piecewise_curve(PiecewiseCurve* curve)
	{
		m_curves.push_back(curve);
	}

	void PiecewiseCurveMgr::remove_piecewise_curve(PiecewiseCurve* curve)
	{
		auto foundIt = std::find(m_curves.begin(), m_curves.end(), curve);

		// Nothing to remove if not in the vector
		if (foundIt == m_curves.end())
			return;

		m_curves.erase(foundIt);
	}


	// Returns the closest curve point/tangent/control point that intersects the given world space ray.
	// Returns nullptr if none were intersected (or false positive. Also, returns the minimum time of
	// intersection in minTime.
	IComponent* PiecewiseCurveMgr::ray_vs_curve_elements(const Ray& ray, float* minTime)
	{
		// TODO: Consider making helper functions for this

		IComponent* closestIntersectedElement = nullptr;
		*minTime = FLT_MAX;

		// For each curve
		for (PiecewiseCurve* currCurve : m_curves)
		{
			const std::vector<SceneNode*>& children = currCurve->get_owner()->get_children();

			// For each possible curve point
			for (SceneNode* childNode : children)
			{
				CurvePoint* curvePoint = childNode->get_component<CurvePoint>();
				if (curvePoint == nullptr)
					continue;

				// Assemble the bounding volume of the point
				AABB pointBv;
				float halfSize = 0.5f * DebugRenderer::s_curvePointSize;
				pointBv.m_min = curvePoint->get_owner()->m_worldTr.m_position - glm::vec3(halfSize, halfSize, halfSize);
				pointBv.m_max = curvePoint->get_owner()->m_worldTr.m_position + glm::vec3(halfSize, halfSize, halfSize);

				// Check the ray against the bv of the current point
				float pointTime = ray_vs_aabb(ray, pointBv);

				// Update if it intersected the point, and the time was less than the current minimum
				if (pointTime >= 0.0f && pointTime < *minTime)
				{
					closestIntersectedElement = curvePoint;
					*minTime = pointTime;
				}


				// For each possible tangent/control point
				for (SceneNode* childNode2 : curvePoint->get_owner()->get_children())
				{
					CurveTangent* tangent = childNode2->get_component<CurveTangent>();
					CurveControlPoint* controlPoint = childNode2->get_component<CurveControlPoint>();

					// If it has a tangent component
					if (tangent)
					{
						// Assemble the bounding volume of the tangent
						AABB tangentBv;
						float halfSize = 0.5f * DebugRenderer::s_tangentEndpointSize;
						tangentBv.m_min = childNode2->m_worldTr.m_position - glm::vec3(halfSize, halfSize, halfSize);
						tangentBv.m_max = childNode2->m_worldTr.m_position + glm::vec3(halfSize, halfSize, halfSize);

						// Check the ray against the bv of the current tangent endpoint
						float tangentTime = ray_vs_aabb(ray, tangentBv);

						// Update if it intersected the tangent endpoint, and the time was less than the current minimum
						if (tangentTime >= 0.0f && tangentTime < *minTime)
						{
							closestIntersectedElement = tangent;
							*minTime = tangentTime;
						}
					}
					// Else if it has a control point component
					else if (controlPoint)
					{
						// Assemble the bounding volume of the control point
						AABB controlPointBv;
						float halfSize = 0.5f * DebugRenderer::s_tangentEndpointSize;
						controlPointBv.m_min = childNode2->m_worldTr.m_position - glm::vec3(halfSize, halfSize, halfSize);
						controlPointBv.m_max = childNode2->m_worldTr.m_position + glm::vec3(halfSize, halfSize, halfSize);

						// Check the ray against the bv of the current control point
						float controlPointTime = ray_vs_aabb(ray, controlPointBv);

						// Update if it intersected the control point, and the time was less than the current minimum
						if (controlPointTime >= 0.0f && controlPointTime < *minTime)
						{
							closestIntersectedElement = controlPoint;
							*minTime = controlPointTime;
						}
					}
				}
			}
		}

		return closestIntersectedElement;
	}
}