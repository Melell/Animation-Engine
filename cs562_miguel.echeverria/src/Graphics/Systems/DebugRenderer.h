/**
* @file DebugRenderer.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief System that provides functions for debug drawing (very expensive, only for debugging).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct Segment;
	struct Triangle;
	struct AABB;
	struct Sphere;
	struct Skin;
	class SceneNode;
	struct Model;


	class DebugRenderer
	{
	public:

		static void draw_point(const glm::vec3& position, const glm::vec4& color, float pointSize);
		static void draw_segment(const Segment& segment, const glm::vec4& color);
		static void draw_aabb(const AABB& aabb, const glm::vec4& color, bool wireframe = false);
		static void draw_aabb(const glm::mat4& m2w, const glm::vec4& color, bool wireframe = false);	// Not tested

		static void draw_curve_node(const glm::vec3& worldPos, const glm::vec4& color);

		static void draw_all_skeletons(const glm::vec4& boneColor, const glm::vec4& jointColor);
		static void draw_joint(const glm::vec3& worldPos, const glm::vec4& color);
		static void draw_skeleton_hierarchy(const glm::vec4& boneColor, const glm::vec4& jointColor, Model* sourceModel, const Skin& skin, std::unordered_map<int, SceneNode*>& modelInstNodes, int rootIdx);


		// The following variables are here for convenience, but in the future they would probably better
		// go inside other classes so that the values can be different per bv, skeleton, or curve

		// Bounding volumes, skeletons, and curves' debug drawing config variables

		static glm::vec4 s_bvsColor;				// BV

		static glm::vec4 s_boneColor;				// Skeleton
		static glm::vec4 s_jointColor;				// Skeleton
		static float s_jointSize;					// Skeleton

		static glm::vec4 s_curveColor;				// Curve
		static glm::vec4 s_curvePointColor;			// Curve
		static float s_curvePointSize;				// Curve
		static glm::vec4 s_tangentLineColor;		// Curve
		static glm::vec4 s_tangentEndpointColor;	// Curve
		static float s_tangentEndpointSize;			// Curve
		static glm::vec4 s_controlPointLineColor;	// Curve
		static glm::vec4 s_controlPointColor;		// Curve
		static float s_controlPointSize;			// Curve
	};
}