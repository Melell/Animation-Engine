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
	struct Mesh
	{
	public:

		Mesh();
		~Mesh();

		// Release all resources allocated by the mesh.
		void clear();

		// Processes the data from a tinygltf model's mesh into this mesh
		void load_mesh_data(const tinygltf::Model& model, const tinygltf::Mesh& mesh);


		std::string m_name;
		std::vector<Primitive> m_primitives;
	};
}