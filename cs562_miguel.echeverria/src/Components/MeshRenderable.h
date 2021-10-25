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
	struct Model;
	class SceneNode;


	class MeshRenderable : public IComponent
	{
	public:

		MeshRenderable();
		virtual ~MeshRenderable();

		// Render the primitives of the mesh this component references, with the transform of the node it belongs to.
		void render_primitives() const;

		void set_mesh_idx(int meshIdx);						// Set the index of the referenced mesh inside the model's vector of meshes
		int get_mesh_idx() const;							// Get the index of the referenced mesh inside the model's vector of meshes

	private:

		int m_meshIdx = -1;

		void on_gui() override;
	};
}