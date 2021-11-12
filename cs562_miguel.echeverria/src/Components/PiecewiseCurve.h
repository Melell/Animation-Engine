/**
* @file PiecewiseCurve.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component for creating piecewise curves.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IComponent.h"


namespace cs460
{
	enum class CURVE_TYPE
	{
		LINEAR = 0,
		HERMITE = 1,
		CATMULL_ROM = 2,
		BEZIER = 3
	};

	struct ArcLengthTableEntry
	{
		float m_param = 0.0f;		// Range [0, 1]
		float m_arcLength = 0.0f;	// Arclength from the beginning of the curve to the point sampled with m_param
	};

	struct SubdivisionEntry
	{
		float m_start = 0.0f;			// Parameter of start of the segment in the curve
		float m_end = 1.0f;				// Parameter of end of the segment in the curve
		int m_subdivisionLevel = 0;		// The subdivision level we are in
	};


	class PiecewiseCurve : public IComponent
	{
	public:

		PiecewiseCurve();
		virtual ~PiecewiseCurve();

		void initialize();
		void update();
		void debug_draw();

		// Builds the arc length table using forward differencing, or adaptive forward differencing.
		void build_arc_length_table(bool useAdaptive);
		void clear_arc_length_table();

		glm::vec3 interpolate_position(float tn, CURVE_TYPE type);
		float get_tn_from_arc_length(float arcLength);
		float get_arc_length_from_tn(float tn);
		

		enum class FINISH_MODE
		{
			STOP,
			RESTART,
			PINPONG
		};

		void change_curve_type(CURVE_TYPE newType);
		void change_finish_mode(FINISH_MODE newMode);

		void reset_animation();

		bool m_drawMovingObject = true;
		bool m_drawCurve = true;

	private:
		// Animation control variables
		glm::vec3 m_currentPos{0.0f, 0.0f, 0.0f};
		CURVE_TYPE m_curveType = CURVE_TYPE::LINEAR;
		SceneNode* m_nodeToMove = nullptr;

		unsigned m_pointCount = 0;
		unsigned m_tangentCount = 0;			// Mainly used for tangent naming, so doesn't need to account for those deleted
		unsigned m_controlPointCount = 0;		// Mainly used for control point naming, so doesn't need to account for those deleted

		float m_currentTime = 0.0f;				// Use in ease-in/out
		float m_totalDuration = 1.0f;			// Use in ease-in/out
		float m_distanceTravelled = 0.0f;		// Used in both
		float m_prevDistance = 0.0f;			// Use in ease-in/out
		float m_totalDistance = 1.0f;			// Used in both
		float m_speed = 0.6f;					// Used in constant speed
		float m_referenceSpeed = 0.41f;			// Used in both
		//float m_timeScale = 1.0f;				// Used in both
		float m_direction = 1.0f;				// Used in both
		FINISH_MODE m_finishMode = FINISH_MODE::RESTART;
		bool m_paused = false;
		bool m_useConstantSpeed = true;
		bool m_syncAnim = true;
		float m_accelerateEndTime = 0.2f;		// Used in ease in/out
		float m_deccelerateStartTime = 0.8f;	// Used in ease in/out

		// Time-value vectors (the point data, updated every frame from the children)
		std::vector<float> m_timeValues;
		std::vector<float> m_propertyValues;


		// Arc-length parametrization table
		std::vector<ArcLengthTableEntry> m_arcLengthTable;
		int m_numberOfSamples = 2;			// For uniform forward differencing
		int m_forcedSubdivisions = 2;		// Number of subdivisions to always do in adaptive, to avoid corner cases
		float m_initialTolerance = 0.25f;	// Initial tolerance for adaptive method
		bool m_useAdaptive = false;
		bool m_showTable = true;			// Show/hide the table on gui
		bool m_drawSamples = true;			// Debug draw the table samples in the curve

		bool m_useFrenetFrame = false;
		bool m_debugDrawFrenetFrame = false;


		void on_gui() override;
		void arc_length_table_on_gui();

		void gather_children_data(std::vector<float>& times, std::vector<float>& values);			// Get the vectors of time, and their respective values
		void search_for_tangents(glm::vec3& inTangent, glm::vec3& outTangent, SceneNode* pointNode);	// Return in inTangent and outTangent the tangent data for pointNode (assuming hermite)
		void search_for_control_points(glm::vec3& leftControlPoint, glm::vec3& rightControlPoint, SceneNode* pointNode);
		void check_bounds();
		void check_distance_bounds();
		void check_timer_bounds();

		void debug_draw_linear();
		void debug_draw_cubic_spline(CURVE_TYPE type);
		void debug_draw_tangents(SceneNode* pointNode);
		void debug_draw_table_samples(CURVE_TYPE type);


		// Performs binary search on the arc lengths of the table, and returns the lower
		// and upper indices for the interpolation in low and high. Also returns -1 if
		// arcLength was not found in the table, and its index in the table if it was found.
		int binary_search_arc_length(float arcLength, int& low, int& high);

		// Does the same as binary_search_arc_length, but for a parameter. The code is not reused
		// because the way the table data structure is organized doesn't allow me to do so.
		int binary_search_parameter(float param, int& low, int& high);

		// Helper functions to build the arc length table using either uniform
		// forward differencing, or adaptive forward differencing.
		void build_table_uniform();
		void build_table_adaptive();

		float get_arc_length_from_tn_uniform(float tn);		// Since params are spaced out uniformally, the index of tn can be retreived directly
		float get_arc_length_from_tn_adaptive(float tn);	// Since params are not spaced out uniformally, binary search needs to be made


		void update_position();		// Move the object along the curve either with constant velocity or with a ease in/out distance-time function
		void update_orientation();	// Orient the character using a basic frenet frame
	};
}