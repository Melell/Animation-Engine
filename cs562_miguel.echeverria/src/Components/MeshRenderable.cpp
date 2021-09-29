/**
* @file MeshRenderable.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Component that references meshes of a specific model, so that the
*		 renderer can render its primitives.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "MeshRenderable.h"
#include "Graphics/Model.h"
#include "Graphics/Primitive.h"


namespace cs460
{
	// Render the primitives of the mesh this component references, with the transform of the node it belongs to.
	void MeshRenderable::render_primitives()
	{
		if (m_model == nullptr)
			return;

		// Get all the primitives of the mesh this component is referencing
		Mesh* mesh = m_model->get_mesh(m_meshIdx);
		std::vector<Primitive>& primitives = mesh->get_all_primitives();
		for (int i = 0; i < primitives.size(); ++i)
		{
			// Shader setup

			// Transformation matrices uniform setup

			// Call render on each primitive
			primitives[i].render();
		}
	}

	void MeshRenderable::on_gui()
	{

	}
}