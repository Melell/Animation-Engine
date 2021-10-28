/**
* @file CurveTangent.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents the endpoint of a tangent in a piecewise curve. Merely for
*		 distinguishing between normal children of a node with a PiecewiseCurve
*		 component, and the childs that represent the tangents of the curve. This
*		 is also used as a control point.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "CurveTangent.h"


namespace cs460
{
	CurveTangent::CurveTangent()
	{
	}

	CurveTangent::~CurveTangent()
	{
	}

	void CurveTangent::set_is_in_tangent(bool isInTangent)
	{
		m_isInTangent = isInTangent;
	}
	bool CurveTangent::get_is_in_tangent() const
	{
		return m_isInTangent;
	}


	void CurveTangent::on_gui()
	{

	}
}