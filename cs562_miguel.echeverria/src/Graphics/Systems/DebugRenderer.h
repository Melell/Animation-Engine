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


	class DebugRenderer
	{
	public:

		static void draw_point(const glm::vec3& position, const glm::vec4& color, float pointSize);
		static void draw_segment(const Segment& segment, const glm::vec4& color);
		static void draw_aabb(const AABB& aabb, const glm::vec4& color, bool wireframe = false);
		static void draw_aabb(const glm::mat4& m2w, const glm::vec3& color, bool wireframe = false);
	};
}