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