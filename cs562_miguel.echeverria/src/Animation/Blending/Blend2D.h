/**
* @file Blend2D.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Type of blend node that allows for blending of multiple animations in
*		 a 2 dimensional space.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IBlendNode.h"


namespace cs460
{
	struct Blend2D : public IBlendNode
	{
    public:

        // Triangle container. Populated in generate_triangles
        std::vector< std::array<unsigned, 3> > m_triangles;
		glm::vec2 m_blendParam{ 0.0f, 0.0f };

		
        // Sets the animation component owner and adds three default childs
        Blend2D(AnimationReference* animCompOwner);

        void produce_pose(float time) override;

        // Generates the triangles using delaunay triangulation 
        // based on the children's blendPosition
        void generate_triangles();

        // Determine in which triangle the param is located and find n0, n1, n2
        // perform barycentric compute to extract a0,a1,a2
        void find_nodes_barycentric(IBlendNode*& node0, IBlendNode*& node1, IBlendNode*& node2, float& a0, float& a1, float& a2);

        // Return the minimum and maximum positions
        glm::vec2 get_min_pos() const;
        glm::vec2 get_max_pos() const;

	private:

        // Performs two dimensional blending using the children nodes.
		void blend_children(float time) override;

        // Returns the z component of the cross product with z=0, but without unnecessary computations
        float cross_2d(const glm::vec2& v0, const glm::vec2& v1) const;
	};
}