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
#include "Components/PiecewiseCurve.h"


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
}