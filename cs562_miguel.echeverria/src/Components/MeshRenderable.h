/**
* @file MeshRenderable.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Component that references meshes of a specific model, so that the
*		 renderer can render its primitives.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"


namespace cs460
{
	class Model;
	class SceneNode;


	class MeshRenderable : public IComponent
	{
	public:

		MeshRenderable();
		~MeshRenderable();

		// Render the primitives of the mesh this component references, with the transform of the node it belongs to.
		void render_primitives() const;

		void set_model_src(Model* model);					// Set the pointer to the model resource that this mesh refers to
		void set_mesh_idx(int meshIdx);						// Set the index of the referenced mesh inside the model's vector of meshes
		void set_model_root_node(SceneNode const* node);	// Set the root node of the model hierarchy (the one with the model instance component)

		Model const* get_model_src() const;					// Get the pointer to the model resource that this mesh refers to
		int get_mesh_idx() const;							// Get the index of the referenced mesh inside the model's vector of meshes
		SceneNode const* get_model_root_node() const;		// Get the root node of the model hierarchy (the one with the model instance component)

	private:

		int m_meshIdx = -1;
		Model* m_model = nullptr;
		SceneNode const* m_modelRootNode = nullptr;

		void on_gui() override;
	};
}