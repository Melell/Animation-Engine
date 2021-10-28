/**
* @file CurvePoint.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a point in a piecewise curve. Merely for distinguishing
*		 between normal children of a node with a PiecewiseCurve component,
*		 and the childs that represent the points of the curve.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IComponent.h"


namespace cs460
{
	class CurvePoint : public IComponent
	{
	public:

		CurvePoint();
		virtual ~CurvePoint();

		// Setter and getter for the time value of this point
		void set_time(float time);
		float get_time() const;

	private:
		float m_time = 0.0f;	// The time of this node. (The position is deduced from the node)

		void on_gui() override;
	};
}