/**
* @file GLTFNode.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Similar to a tinygltf::Node, used to store the nodes in a model as "archetypes".
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "GLTFNode.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	// Loads the data from the give gltf node to our own
	void GLTFNode::load_node_data(const tinygltf::Model& model, int nodeIdx, std::map<int, int>& skinNodes)
	{
		const tinygltf::Node& node = model.nodes[nodeIdx];

		m_name = node.name;
		m_meshIdx = node.mesh;
		m_skinIdx = node.skin;
		m_childrenIndices = node.children;
		set_tr_data(node);

		if (m_skinIdx > 0)
			skinNodes[m_skinIdx] = nodeIdx;
	}


	void GLTFNode::set_tr_data(const tinygltf::Node& node)
	{
		bool locationSet = false;
		bool scaleSet = false;
		bool orientationSet = false;

		// Set the local position, scale and orientation from the gltf node data if it was provided
		if (node.translation.size())
		{
			m_localTransform.m_position.x = (float)node.translation[0];
			m_localTransform.m_position.y = (float)node.translation[1];
			m_localTransform.m_position.z = (float)node.translation[2];
			locationSet = true;
		}
		if (node.scale.size())
		{
			m_localTransform.m_scale.x = (float)node.scale[0];
			m_localTransform.m_scale.y = (float)node.scale[1];
			m_localTransform.m_scale.z = (float)node.scale[2];
			scaleSet = true;
		}
		if (node.rotation.size())
		{
			m_localTransform.m_orientation.x = (float)node.rotation[0];
			m_localTransform.m_orientation.y = (float)node.rotation[1];
			m_localTransform.m_orientation.z = (float)node.rotation[2];
			m_localTransform.m_orientation.w = (float)node.rotation[3];
			orientationSet = true;
		}


		if (node.matrix.size())
		{
			// Store the matrix
			glm::mat4 modelToLocalMtx(1.0f);
			for (int i = 0; i < node.matrix.size(); i += 4)
				modelToLocalMtx[i / 4] = glm::vec4(node.matrix[i], node.matrix[i + 1], node.matrix[i + 2], node.matrix[i + 3]);

			glm::vec3 position;
			glm::quat orientation;
			glm::vec3 scale;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(modelToLocalMtx, scale, orientation, position, skew, perspective);

			// Store the position extracted from the matrix if it hasn't already been provided if it hasn't already been provided
			if (!locationSet)
				m_localTransform.m_position = position;
			// Store the scale extracted from the matrix if it hasn't already been provided if it hasn't already been provided
			if (!scaleSet)
				m_localTransform.m_scale = scale;
			// Store the orientation extracted from the matrix if it hasn't already been provided if it hasn't already been provided
			if (!orientationSet)
				m_localTransform.m_orientation = orientation;
		}
	}
}