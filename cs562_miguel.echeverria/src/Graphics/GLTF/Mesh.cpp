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
		m_boundingVolume.m_min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		m_boundingVolume.m_max = glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	}


	Mesh::~Mesh()
	{
		clear();
	}


	// Release all resources allocated by the mesh.
	void Mesh::clear()
	{
		for (int i = 0; i < m_primitives.size(); ++i)
			m_primitives[i].delete_gl_buffers();

		m_primitives.clear();
	}


	// Processes the data from a tinygltf model's mesh into this mesh
	void Mesh::load_mesh_data(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
	{
		m_name = mesh.name;

		m_primitives.resize(mesh.primitives.size());
		for (int i = 0; i < m_primitives.size(); ++i)
		{
			m_primitives[i].load_primitive_data(model, mesh.primitives[i]);


			// Update the bounding volume
			const glm::vec3& currentMin = m_primitives[i].get_min_pos();
			const glm::vec3& currentMax = m_primitives[i].get_max_pos();

			// Update the min position
			if (currentMin.x < m_boundingVolume.m_min.x)
				m_boundingVolume.m_min.x = currentMin.x;
			if (currentMin.y < m_boundingVolume.m_min.y)
				m_boundingVolume.m_min.y = currentMin.y;
			if (currentMin.z < m_boundingVolume.m_min.z)
				m_boundingVolume.m_min.z = currentMin.z;

			// Update the max position
			if (currentMax.x < m_boundingVolume.m_max.x)
				m_boundingVolume.m_max.x = currentMax.x;
			if (currentMax.y < m_boundingVolume.m_max.y)
				m_boundingVolume.m_max.y = currentMax.y;
			if (currentMax.z < m_boundingVolume.m_max.z)
				m_boundingVolume.m_max.z = currentMax.z;
		}
	}
}