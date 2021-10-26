/**
* @file Model.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Resource that holds information about a gltf model like the filename.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Mesh.h"
#include "Animation/Skin.h"
#include "Composition/GLTFNode.h"
#include "Composition/GLTFScene.h"
#include "Animation/Animation.h"


namespace tinygltf
{
	class Model;
}


namespace cs460
{
	struct Model
	{
		Model();
		~Model();

		// Loads the given gltf file and stores all its data
		void load_gltf_file(const std::string& filePath);

		// Process the tinygltf model structure into our own
		void load_model_data(const tinygltf::Model& model);

		// Releases all the resources used by the meshes
		void clear();

		// Compare two models based on their filename
		bool operator==(const Model& other) const;


		std::string m_fileName;
		std::string m_filePath;
		std::vector<GLTFScene> m_scenes;
		std::vector<GLTFNode> m_nodes;
		std::vector<Mesh> m_meshes;
		std::vector<Skin> m_skins;
		std::vector<Animation> m_animations;
		int m_defaultScene;
	};
}