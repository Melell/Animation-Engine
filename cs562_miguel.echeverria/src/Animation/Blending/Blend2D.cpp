/**
* @file Blend2D.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Type of blend node that allows for blending of multiple animations in
*		 a 2 dimensional space.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Blend2D.h"
#include "Math/Geometry/Delaunator.hpp"
#include "BlendAnim.h"
#include "Components/Animation/AnimationReference.h"
#include "Composition/SceneNode.h"
#include "Graphics/GLTF/Model.h"


namespace cs460
{
	// Sets the animation component owner and adds three default childs
	Blend2D::Blend2D(AnimationReference* animCompOwner)
		:	IBlendNode(animCompOwner)
	{
		BlendAnim* animNode0 = static_cast<BlendAnim*>(add_child(BlendNodeTypes::BLEND_ANIM));
		animNode0->m_blendPos = glm::vec2(-0.5f, -0.5f);
		animNode0->m_animSource = &m_animCompOwner->get_owner()->get_model()->m_animations[0];
		
		BlendAnim* animNode1 = static_cast<BlendAnim*>(add_child(BlendNodeTypes::BLEND_ANIM));
		animNode1->m_blendPos = glm::vec2(0.5f, -0.5f);
		animNode1->m_animSource = &m_animCompOwner->get_owner()->get_model()->m_animations[0];
		
		BlendAnim* animNode2 = static_cast<BlendAnim*>(add_child(BlendNodeTypes::BLEND_ANIM));
		animNode2->m_blendPos = glm::vec2(0.0f, 0.5f);
		animNode2->m_animSource = &m_animCompOwner->get_owner()->get_model()->m_animations[0];
	}


	void Blend2D::produce_pose(float time)
	{
		if (m_children.empty())
			return;

		blend_children(time);
	}


	// Generates the triangles using delaunay triangulation 
	// based on the children's blendPosition
	void Blend2D::generate_triangles()
	{
		// Prepare all the blend coordinates for the triangulation
		std::vector<double> childCoords;
		childCoords.resize(m_children.size() * 2);

		for (int i = 0; i < m_children.size(); ++i)
		{
			childCoords[i * 2] = (double)m_children[i]->m_blendPos.x;
			childCoords[1 + i * 2] = (double)m_children[i]->m_blendPos.y;
		}

		// Perform delaunay triangulation
		delaunator::Delaunator triangulator(childCoords);

		// Store the triangles in our own data structure
		size_t numberOfTriangles = triangulator.triangles.size() / 3;
		m_triangles.resize(numberOfTriangles);
		for (int i = 0; i < numberOfTriangles; ++i)
		{
			m_triangles[i][0] = (unsigned)triangulator.triangles[i * 3];
			m_triangles[i][1] = (unsigned)triangulator.triangles[i * 3 + 1];
			m_triangles[i][2] = (unsigned)triangulator.triangles[i * 3 + 2];
		}
	}

	// Determine in which triangle the param is located and find n0, n1, n2
	// perform barycentric compute to extract a0,a1,a2
	void Blend2D::find_nodes_barycentric(IBlendNode*& node0, IBlendNode*& node1, IBlendNode*& node2, float& a0, float& a1, float& a2)
	{
		for (int i = 0; i < m_triangles.size(); ++i)
		{
			const auto& indices = m_triangles[i];
			glm::vec2 v0(m_children[indices[0]]->m_blendPos.x, m_children[indices[0]]->m_blendPos.y);
			glm::vec2 v1(m_children[indices[1]]->m_blendPos.x, m_children[indices[1]]->m_blendPos.y);
			glm::vec2 v2(m_children[indices[2]]->m_blendPos.x, m_children[indices[2]]->m_blendPos.y);

			float area0 = cross_2d(v1 - m_blendParam, v2 - m_blendParam);
			if (area0 > 0.0f)
				continue;

			float area1 = cross_2d(v2 - m_blendParam, v0 - m_blendParam);
			if (area1 > 0.0f)
				continue;

			float area2 = cross_2d(v0 - m_blendParam, v1 - m_blendParam);
			if (area2 > 0.0f)
				continue;

			// Return the nodes of the triangle that blend param is contained in
			node0 = m_children[indices[0]];
			node1 = m_children[indices[1]];
			node2 = m_children[indices[2]];

			float totalArea = cross_2d(v1 - v0, v2 - v0);

			// Return the barycentric coordinates of blend param
			a0 = area0 / totalArea;
			a1 = area1 / totalArea;
			a2 = area2 / totalArea;

			return;
		}
	}


	// Return the minimum and maximum positions
	glm::vec2 Blend2D::get_min_pos() const
	{
		if (m_triangles.empty())
			return glm::vec2(-1.0f, -1.0f);

		float minX = FLT_MAX;
		float minY = FLT_MAX;

		// For every triangle
		for (int i = 0; i < m_children.size(); ++i)
		{
			// Get the vertices of the current triangle
			const glm::vec2& blendPos = m_children[i]->m_blendPos;

			// Update minimum x
			if (blendPos.x < minX)
				minX = blendPos.x;

			// Update minimum y
			if (blendPos.y < minY)
				minY = blendPos.y;
		}

		return glm::vec2(minX, minY);
	}

	glm::vec2 Blend2D::get_max_pos() const
	{
		if (m_triangles.empty())
			return glm::vec2(1.0f, 1.0f);

		float maxX = -FLT_MAX;
		float maxY = -FLT_MAX;

		// For every triangle
		for (int i = 0; i < m_children.size(); ++i)
		{
			// Get the vertices of the current triangle
			const glm::vec2& blendPos = m_children[i]->m_blendPos;

			// Update minimum x
			if (blendPos.x > maxX)
				maxX = blendPos.x;

			// Update minimum y
			if (blendPos.y > maxY)
				maxY = blendPos.y;
		}

		return glm::vec2(maxX, maxY);
	}


	// Performs two dimensional blending using the children nodes.
	void Blend2D::blend_children(float time)
	{
		// Generate the triangles in case they have changed
		generate_triangles();

		// we assume that the children are sorted incrementally by their blend position
		IBlendNode* node0, * node1, * node2;
		node0 = node1 = node2 = nullptr;
		float a0, a1, a2;
		a0 = a1 = a2 = 0.0f;
		find_nodes_barycentric(node0, node1, node2, a0, a1, a2);

		if (!node0 || !node1 || !node2)
			return;

		// produce the pose from each
		node0->produce_pose(time);
		node1->produce_pose(time);
		node2->produce_pose(time);

		// blend the pose into ours
		blend_pose_barycentric(
			node0->m_pose, node1->m_pose, node2->m_pose,
			a0, a1, a2,
			m_pose,
			nullptr);
	}


	// Returns the z component of the cross product with z=0, but without unnecessary computations
	float Blend2D::cross_2d(const glm::vec2& v0, const glm::vec2& v1) const
	{
		return v0.x * v1.y - v0.y * v1.x;
	}
}