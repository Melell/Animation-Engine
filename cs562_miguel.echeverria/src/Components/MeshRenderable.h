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


	class MeshRenderable : public IComponent
	{
	public:

		// Render the primitives of the mesh this component references, with the transform of the node it belongs to.
		void render_primitives();

	private:

		int m_meshIdx;
		Model* m_model;

		void on_gui() override;
	};
}