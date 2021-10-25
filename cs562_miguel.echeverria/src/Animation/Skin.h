/**
* @file Skin.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a skin object from gltf.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

namespace tinygltf
{
	class Model;
	struct Skin;
}


namespace cs460
{
	struct Skin
	{
		void load_skin_data(const tinygltf::Model& model, int skinIdx, const std::map<int, int>& skinNodes);


		std::string m_name;
		std::vector<glm::mat4> m_invBindMatrices;
		std::vector<int> m_joints;
		int m_commonRootIdx = 0;


	private:
		void load_inv_bind_matrices(const tinygltf::Model& model, const tinygltf::Skin& skin);
	};
}