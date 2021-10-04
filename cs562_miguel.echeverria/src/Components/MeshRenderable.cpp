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
#include "Graphics/Shader.h"
#include "Composition/SceneNode.h"
#include "Composition/Scene.h"


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

		Scene& scene = Scene::get_instance();

		// Get all the primitives of the mesh this component is referencing
		Mesh* mesh = m_model->get_mesh(m_meshIdx);
		std::vector<Primitive>& primitives = mesh->get_all_primitives();
		for (int i = 0; i < primitives.size(); ++i)
		{
			const glm::mat4& modelToWorld = get_owner()->m_modelToWorldMtx;
			const glm::mat4& worldToView = scene.get_camera().get_view_mtx();
			const glm::mat4& perspectiveProjection = scene.get_camera().get_projection_mtx();

			// Decide between phong and phong_texture
			Shader* shader = primitives[i].get_shader();
			const Material& material = primitives[i].get_material();
			if (material.m_usesTexture)
				shader = primitives[i].set_shader("phong_texture");
			else
				shader = primitives[i].set_shader("phong_color");

			// Shader and uniform setup
			shader->use();
			shader->set_uniform("modelToWorld", modelToWorld);				// Set model to world
			shader->set_uniform("worldToView", worldToView);				// Set the view mtx
			shader->set_uniform("perspectiveProj", perspectiveProjection);	// Set the perspective projection matrix
			shader->set_uniform("normalViewMtx", glm::transpose(glm::inverse(glm::mat3(worldToView * modelToWorld))));

			// Set the light properties
			shader->set_uniform("light.m_direction", scene.m_lightProperties.m_direction);
			shader->set_uniform("light.m_ambient", scene.m_lightProperties.m_ambient);
			shader->set_uniform("light.m_diffuse", scene.m_lightProperties.m_diffuse);
			shader->set_uniform("light.m_specular", scene.m_lightProperties.m_specular);

			// Set the material properties
			if (material.m_usesTexture)
				shader->set_uniform("mat.m_diffuse", material.m_baseColorTex.get_texture_unit());
			else
				shader->set_uniform("mat.m_diffuse", glm::vec3(material.m_baseColor));

			shader->set_uniform("mat.m_shininess", 32.0f);
			
			// Call render on each primitive
			primitives[i].render();
		}
	}

	// Set the pointer to the model resource that this mesh refers to
	void MeshRenderable::set_model_src(Model* model)
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