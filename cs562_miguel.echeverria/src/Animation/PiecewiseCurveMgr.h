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
	struct Ray;
	class IComponent;


	class PiecewiseCurveMgr
	{
	public:

		static PiecewiseCurveMgr& get_instance();
		~PiecewiseCurveMgr();

		void update();			// Updates all the piecewise curve components in order
		void debug_draw();		// Uses debug drawing to render each curve

		void add_piecewise_curve(PiecewiseCurve* curve);
		void remove_piecewise_curve(PiecewiseCurve* curve);

		// Returns the closest curve point/tangent/control point that intersects the given world space ray.
		// Returns nullptr if none were intersected (or false positive. Also, returns the minimum time of
		// intersection in minTime.
		IComponent* ray_vs_curve_elements(const Ray& ray, float* minTime);

	private:
		std::vector<PiecewiseCurve*> m_curves;

		PiecewiseCurveMgr();
		PiecewiseCurveMgr(const PiecewiseCurveMgr&) = delete;
		PiecewiseCurveMgr& operator=(const PiecewiseCurveMgr&) = delete;
	};
}