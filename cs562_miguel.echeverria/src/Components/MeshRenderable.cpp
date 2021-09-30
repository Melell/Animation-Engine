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
#include "Graphics/Renderer.h"


namespace cs460
{
	MeshRenderable::MeshRenderable()
	{
		Renderer::get_instance().add_mesh_renderable(this);
	}

	MeshRenderable::~MeshRenderable()
	{
	}

	// Render the primitives of the mesh this component references, with the transform of the node it belongs to.
	void MeshRenderable::render_primitives() const
	{
		if (m_model == nullptr)
			return;

		// Get all the primitives of the mesh this component is referencing
		Mesh const * mesh = m_model->get_mesh(m_meshIdx);
		const std::vector<Primitive>& primitives = mesh->get_all_primitives();
		for (int i = 0; i < primitives.size(); ++i)
		{
			// TODO: Shader setup

			// TODO: Transformation matrices uniform setup

			// Call render on each primitive
			primitives[i].render();
		}
	}

	// Set the pointer to the model resource that this mesh refers to
	void MeshRenderable::set_model_src(Model const* model)
	{
		m_model = model;
	}

	// Set the index of the referenced mesh inside the model's vector of meshes
	void MeshRenderable::set_mesh_idx(int meshIdx)
	{
		m_meshIdx = meshIdx;
	}

	// Set the root node of the model hierarchy (the one with the model instance component)
	void MeshRenderable::set_model_root_node(SceneNode const* node)
	{
		m_modelRootNode = node;
	}

	// Get the pointer to the model resource that this mesh refers to
	Model const* MeshRenderable::get_model_src() const
	{
		return m_model;
	}

	// Get the index of the referenced mesh inside the model's vector of meshes
	int MeshRenderable::get_mesh_idx() const
	{
		return m_meshIdx;
	}

	// Get the root node of the model hierarchy (the one with the model instance component)
	SceneNode const* MeshRenderable::get_model_root_node() const
	{
		return m_modelRootNode;
	}

	void MeshRenderable::on_gui()
	{

	}
}