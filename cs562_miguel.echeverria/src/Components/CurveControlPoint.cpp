/**
* @file CurveControlPoint.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a control point of a bezier curve. Merely for distinguishing
*		 between normal children of a node with a PiecewiseCurve component, and
*		 the childs that represent the control points of the curve.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "CurveControlPoint.h"


namespace cs460
{
	CurveControlPoint::CurveControlPoint()
	{
	}

	CurveControlPoint::~CurveControlPoint()
	{
	}


	void CurveControlPoint::set_is_left_control_point(bool isLeftControlPoint)
	{
		m_isLeftControlPoint = isLeftControlPoint;
	}
	bool CurveControlPoint::get_is_left_control_point() const
	{
		return m_isLeftControlPoint;
	}


	void CurveControlPoint::on_gui()
	{
	}
}