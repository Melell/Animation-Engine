/**
* @file Mesh.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Resource that holds a collection of primitives that can be drawn.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Mesh.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	Mesh::Mesh()
	{
	}


	Mesh::~Mesh()
	{
		clear();
	}


	// Processes the data from a tinygltf model's mesh into this mesh
	void Mesh::process_mesh_data(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
	{
		m_name = mesh.name;

		m_primitives.resize(mesh.primitives.size());
		for (int i = 0; i < m_primitives.size(); ++i)
		{
			m_primitives[i].process_primitive_data(model, mesh.primitives[i]);
		}
	}

	Primitive const* Mesh::get_primitve(int index) const
	{
		if (index < 0 || index >= m_primitives.size())
			return nullptr;

		return &m_primitives[index];
	}

	std::vector<Primitive>& Mesh::get_all_primitives()
	{
		return m_primitives;
	}

	// Release all resources allocated by the mesh.
	void Mesh::clear()
	{
		for (int i = 0; i < m_primitives.size(); ++i)
			m_primitives[i].delete_gl_buffers();

		m_primitives.clear();
	}


	// Getter and setter for the name
	void Mesh::set_name(const std::string& newName)
	{
		m_name = newName;
	}

	std::string Mesh::get_name() const
	{
		return m_name;
	}
}