/**
* @file PiecewiseCurveMgr.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Updates and draws all the piecewise curve components.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class PiecewiseCurve;


	class PiecewiseCurveMgr
	{
	public:

		static PiecewiseCurveMgr& get_instance();
		~PiecewiseCurveMgr();

		void update();			// Updates all the piecewise curve components in order
		void debug_draw();		// Uses debug drawing to render each curve

		void add_piecewise_curve(PiecewiseCurve* curve);
		void remove_piecewise_curve(PiecewiseCurve* curve);

	private:
		std::vector<PiecewiseCurve*> m_curves;

		PiecewiseCurveMgr();
		PiecewiseCurveMgr(const PiecewiseCurveMgr&) = delete;
		PiecewiseCurveMgr& operator=(const PiecewiseCurveMgr&) = delete;
	};
}