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
	struct IKChain;


	class DebugRenderer
	{
	public:

		static void draw_point(const glm::vec3& position, const glm::vec4& color, float pointSize);
		static void draw_segment(const Segment& segment, const glm::vec4& color);
		static void draw_aabb(const AABB& aabb, const glm::vec4& color, bool wireframe = false);
		static void draw_aabb(const glm::mat4& m2w, const glm::vec4& color, bool wireframe = false);
		static void draw_triangle(const Triangle& triangle, const glm::vec4& color, bool wireframe = false);
		static void draw_triangle_shaded(const Triangle& triangle, const glm::vec4& color);

		static void draw_curve_node(const glm::vec3& worldPos, const glm::vec4& color, float size);

		static void draw_all_skeletons(const glm::vec4& boneColor, const glm::vec4& jointColor, float jointSize);
		static void draw_joint(const glm::vec3& worldPos, const glm::vec4& color, float jointSize);
		static void draw_skeleton_hierarchy(const glm::vec4& boneColor, const glm::vec4& jointColor, float joinSize, Model* sourceModel, const Skin& skin, std::unordered_map<int, SceneNode*>& modelInstNodes, int rootIdx);

		static void draw_grid(float worldXSize, float worldZSize, unsigned xSubdivisions, unsigned zSubdivisions);

		static void draw_all_ik_chains();
		static void draw_ik_chain(IKChain* chain, const glm::vec4& boneColor, const glm::vec4& boneHighlightColor, const glm::vec4& jointColor, const glm::vec4& targetColor);
		static void draw_ik_bone(const glm::vec3& pos, const glm::vec3& parentPos, const glm::vec3* pyramidBaseVerts, const glm::vec4& boneColor, const glm::vec4& boneHighlightColor);


		// The following variables are here for convenience, but in the future they would probably better
		// go inside other classes so that the values can be different per bv, skeleton, or curve

		// Bounding volumes, skeletons, and curves' debug drawing config variables

		static glm::vec4 s_bvsColor;				// BV

		static bool s_enableSkeletonDrawing;		// Skeleton
		static glm::vec4 s_boneColor;				// Skeleton
		static glm::vec4 s_jointColor;				// Skeleton
		static float s_jointSize;					// Skeleton


		static bool s_enableMovingObjectDrawing;	// Curve (moving object)
		static glm::vec4 s_movingObjectColor;		// Curve (moving object)
		static float s_movingObjectSize;			// Curve (moving object)

		static bool s_enableCurveDrawing;			// Curve (curve and curve points)
		static glm::vec4 s_curveColor;				// Curve (curve and curve points)
		static glm::vec4 s_curvePointColor;			// Curve (curve and curve points)
		static float s_curvePointSize;				// Curve (curve and curve points)

		static bool s_enableTangentDrawing;			// Curve (tangents/control points)
		static glm::vec4 s_tangentLineColor;		// Curve (tangents/control points)
		static glm::vec4 s_tangentEndpointColor;	// Curve (tangents/control points)
		static float s_tangentEndpointSize;			// Curve (tangents/control points)

		static bool s_enableTableSamplesDrawing;	// Curve (arc length table)
		static glm::vec4 s_tableSampleColor;		// Curve (arc length table)
		static float s_tableSampleSize;				// Curve (arc length table)


		static bool s_enableGridDrawing;			// Debug Grid
		static float s_xGridSize;					// Debug Grid
		static float s_zGridSize;					// Debug Grid
		static unsigned s_xSubdivisions;			// Debug Grid
		static unsigned s_zSubdivisions;			// Debug Grid


		static bool s_enableIKChainDrawing;			// IKChain
		static glm::vec4 s_ikBoneColorIdle;			// IKChain
		static glm::vec4 s_ikBoneColorProcessing;	// IKChain
		static glm::vec4 s_ikBoneColorSuccess;		// IKChain
		static glm::vec4 s_ikBoneColorFailure;		// IKChain
		static glm::vec4 s_ikBoneHighlightColor;	// IKChain
		static glm::vec4 s_ikJointColor;			// IKChain
		static glm::vec4 s_ikTargetColor;			// IKChain
	};
}