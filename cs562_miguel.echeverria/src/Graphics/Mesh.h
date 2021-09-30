/**
* @file Mesh.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Resource that holds a collection of primitives that can be drawn.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Primitive.h"

namespace tinygltf
{
	class Model;
	struct Mesh;
}


namespace cs460
{
	class Mesh
	{
	public:

		Mesh();
		~Mesh();

		// Processes the data from a tinygltf model's mesh into this mesh
		void process_mesh_data(const tinygltf::Model& model, const tinygltf::Mesh& mesh);

		Primitive const* get_primitve(int index) const;
		const std::vector<Primitive>& get_all_primitives() const;

		// Release all resources allocated by the mesh.
		void clear();

		// Getter and setter for the name
		void set_name(const std::string& newName);
		std::string get_name() const;

	private:

		std::string m_name;
		std::vector<Primitive> m_primitives;
	};
}