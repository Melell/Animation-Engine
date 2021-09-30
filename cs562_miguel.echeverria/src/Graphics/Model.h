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

namespace tinygltf
{
	class Model;
}


namespace cs460
{
	class Model
	{
	public:

		Model();
		~Model();

		// Loads a gltf file (only meshes, primitives and material, but not the nodes)
		void load_gltf(const std::string& filePath);

		Mesh const* get_mesh(int index) const;

		// Releases all the resources used by the meshes
		void clear();

		// Compare two models based on their filename
		bool operator==(const Model& other) const;

		// Getters for the filename and path to the file
		std::string get_filename() const;
		std::string get_filepath() const;

	private:

		std::string m_fileName;
		std::string m_filePath;
		std::vector<Mesh> m_meshes;

		// Process the tinygltf model structure into our own
		void process_model_data(const tinygltf::Model& model);
	};
}