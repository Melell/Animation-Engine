/**
* @file Model.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Resource that holds information about a gltf model like the filename.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Model.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	Model::Model()
		:	m_defaultScene(0)
	{
	}


	Model::~Model()
	{
		clear();
	}

	// Loads the given gltf file and stores all its data
	void Model::load_gltf_file(const std::string& filePath)
	{
		// Store the filepath and filename
		m_filePath = filePath;
		fs::path path(filePath);
		m_fileName = path.filename().generic_string();

		// Use tinygltf to parse the file and get the data
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string errorStr;
		std::string warningStr;

		bool ret = loader.LoadASCIIFromFile(&model, &errorStr, &warningStr, m_filePath);
		//bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

		if (!warningStr.empty())
			std::cout << "TINYGLTF WARNING: " << warningStr << std::endl;

		if (!errorStr.empty())
			std::cout << "TINYGLTF ERROR: " << errorStr << std::endl;

		if (!ret) {
			std::cout << "TINYGLTF: Failed to parse glTF\n";
			return;
		}

		load_model_data(model);
	}


	// Process the tinygltf model structure into our own
	void Model::load_model_data(const tinygltf::Model& model)
	{
		// Reallocate the vector of meshes, skins etc with enough size
		clear();
		m_scenes.resize(model.scenes.size());
		m_nodes.resize(model.nodes.size());
		m_meshes.resize(model.meshes.size());
		m_skins.resize(model.skins.size());
		m_animations.resize(model.animations.size());

		// Associates skin index to index of node that referenced the skin
		std::map<int, int> skinNodes;

		m_defaultScene = model.defaultScene > 0 ? model.defaultScene : 0;

		// Load the scenes
		for (int i = 0; i < m_scenes.size(); ++i)
			m_scenes[i].load_scene_data(model, model.scenes[i]);

		// Load the nodes
		for (int i = 0; i < m_nodes.size(); ++i)
			m_nodes[i].load_node_data(model, i, skinNodes);

		// Load the meshes
		for (int i = 0; i < m_meshes.size(); ++i)
			m_meshes[i].load_mesh_data(model, model.meshes[i]);

		// Load the skins
		for (int i = 0; i < m_skins.size(); ++i)
			m_skins[i].load_skin_data(model, i, skinNodes);

		// Load the animations
		for (int i = 0; i < m_animations.size(); ++i)
			m_animations[i].load_animation_data(model, model.animations[i]);
	}


	// Releases all the resources used by the meshes
	void Model::clear()
	{
		for (int i = 0; i < m_meshes.size(); ++i)
			m_meshes[i].clear();

		m_scenes.clear();
		m_nodes.clear();
		m_meshes.clear();
		m_skins.clear();
	}

	// Compare two models based on their filename
	bool Model::operator==(const Model& other) const
	{
		return m_fileName == other.m_fileName;
	}
}