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
	{

	}


	Model::~Model()
	{
		clear();
	}

	// Loads a gltf file (only meshes, primitives and material, but not the nodes)
	void Model::load_gltf(const std::string& filePath)
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

		process_model_data(model);
	}

	Mesh* Model::get_mesh(int index)
	{
		if (index < 0 || index >= m_meshes.size())
			return nullptr;

		return &m_meshes[index];
	}

	// Releases all the resources used by the meshes
	void Model::clear()
	{
		for (int i = 0; i < m_meshes.size(); ++i)
			m_meshes[i].clear();

		m_meshes.clear();
	}

	// Compare two models based on their filename
	bool Model::operator==(const Model& other) const
	{
		return m_fileName == other.m_fileName;
	}


	std::string Model::get_filename() const
	{
		return m_fileName;
	}
	std::string Model::get_filepath() const
	{
		return m_filePath;
	}


	// Process the tinygltf model structure into our own
	void Model::process_model_data(const tinygltf::Model& model)
	{
		// Reallocate the vector of meshes with enough size
		clear();
		m_meshes.resize(model.meshes.size());

		// Process the data of each tinygltf mesh into our own
		for (int i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i].process_mesh_data(model, model.meshes[i]);
		}
	}
}