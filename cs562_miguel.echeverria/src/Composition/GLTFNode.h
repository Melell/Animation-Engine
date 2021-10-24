/**
* @file GLTFNode.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Similar to a tinygltf::Node, used to store the nodes in a model as "archetypes".
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/


#pragma once

namespace tinygltf
{
	class Model;
	class Node;
}


namespace cs460
{
	struct GLTFNode
	{
		// Loads the data from the give gltf node to our own
		void load_node_data(const tinygltf::Model& model, const tinygltf::Node& node);
		void set_tr_data(const tinygltf::Node& node);


		std::string m_name;
		std::vector<int> m_childrenIndices;
		int m_meshIdx = -1;
		int m_skinIdx = -1;
		glm::vec3 m_position{0.0f, 0.0f, 0.0f};
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };
		glm::quat m_orientation{ 1.0f, 0.0f, 0.0f, 0.0f };
	};
}