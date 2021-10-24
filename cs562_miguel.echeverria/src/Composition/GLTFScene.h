/**
* @file GLTFScene.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Similar to a tinygltf::Scene, holds the indices of the parent nodes in the scene.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

namespace tinygltf
{
	class Model;
	struct Scene;
}


namespace cs460
{
	struct GLTFScene
	{
		// Loads the data from the gltf scene to our own
		void load_scene_data(const tinygltf::Model& model, const tinygltf::Scene& scene);


		std::string m_name;
		std::vector<int> m_nodeIndices;
	};
}