/**
* @file CurvePoint.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a point in a piecewise curve. Merely for distinguishing
*		 between normal children of a node with a PiecewiseCurve component,
*		 and the childs that represent the points of the curve.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "CurvePoint.h"


namespace cs460
{
	CurvePoint::CurvePoint()
	{
	}

	CurvePoint::~CurvePoint()
	{
	}


	// Setter and getter for the time value of this point
	void CurvePoint::set_time(float time)
	{
		m_time = time;
	}

	float CurvePoint::get_time() const
	{
		return m_time;
	}


	void CurvePoint::on_gui()
	{
		ImGui::InputFloat("Global Time", &m_time);
	}
}