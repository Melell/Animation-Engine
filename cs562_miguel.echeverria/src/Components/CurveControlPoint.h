/**
* @file CurveControlPoint.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a control point of a bezier curve. Merely for distinguishing
*		 between normal children of a node with a PiecewiseCurve component, and
*		 the childs that represent the control points of the curve.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IComponent.h"


namespace cs460
{
	class CurveControlPoint : public IComponent
	{
	public:

		CurveControlPoint();
		virtual ~CurveControlPoint();

		void set_is_left_control_point(bool isLeftControlPoint);
		bool get_is_left_control_point() const;

	private:
		bool m_isLeftControlPoint = false;

		void on_gui() override;
	};
}