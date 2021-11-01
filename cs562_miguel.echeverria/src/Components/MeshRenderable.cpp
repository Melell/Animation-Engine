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
#include "Graphics/GLTF/Model.h"
#include "Graphics/GLTF/Primitive.h"
#include "Graphics/Systems/Renderer.h"
#include "Graphics/Rendering/Shader.h"
#include "Composition/SceneNode.h"
#include "Composition/Scene.h"
#include "SkinReference.h"
#include "Platform/InputMgr.h"


namespace cs460
{
	MeshRenderable::MeshRenderable()
	{
		Renderer::get_instance().add_mesh_renderable(this);
	}

	MeshRenderable::~MeshRenderable()
	{
		//std::cout << "MESH RENDERABLE DESTRUCTOR\n";
		Renderer::get_instance().remove_mesh_renderable(this);
	}

	// Render the primitives of the mesh this component references, with the transform of the node it belongs to.
	void MeshRenderable::render_primitives() const
	{
		Model* modelResource = get_owner()->get_model();

		if (modelResource == nullptr)
			return;

		Scene& scene = Scene::get_instance();
		InputMgr& inputMgr = InputMgr::get_instance();

		// Get all the primitives of the mesh this component is referencing
		Mesh& mesh = modelResource->m_meshes[m_meshIdx];
		std::vector<Primitive>& primitives = mesh.m_primitives;
		for (int i = 0; i < primitives.size(); ++i)
		{
			// Should this go here?
			bool useNormalMap = true;
			if (inputMgr.is_key_down(KEYS::key_n))
				useNormalMap = !useNormalMap;


			const glm::mat4& modelToWorld = get_owner()->m_worldTr.get_model_mtx();
			const glm::mat4& worldToView = scene.get_camera().get_view_mtx();
			const glm::mat4& perspectiveProjection = scene.get_camera().get_projection_mtx();

			// Decide between phong color, phong texture, and phong normal map
			Shader* shader = primitives[i].get_shader();
			const Material& material = primitives[i].get_material();
			if (material.m_usesNormalTexture && useNormalMap)
				shader = primitives[i].set_shader("phong_normal_map");
			else if (material.m_usesBaseTexture)
				shader = primitives[i].set_shader("phong_texture");
			else
				shader = primitives[i].set_shader("phong_color");

			// Shader and uniform setup
			shader->use();
			shader->set_uniform("modelToWorld", modelToWorld);				// Set model to world
			shader->set_uniform("worldToView", worldToView);				// Set the view mtx
			shader->set_uniform("perspectiveProj", perspectiveProjection);	// Set the perspective projection matrix
			shader->set_uniform("normalViewMtx", glm::transpose(glm::inverse(glm::mat3(worldToView * modelToWorld))));

			// Set the joint matrices (if the mesh has a skin)
			if (SkinReference* skin = get_owner()->get_component<SkinReference>())
			{
				shader->set_uniform("useSkinning", true);

				const std::vector<glm::mat4>& jointMatrices = skin->get_joint_matrices();
				for (int j = 0; j < jointMatrices.size(); ++j)
					shader->set_uniform("jointMatrices[" + std::to_string(j) + "]", jointMatrices[j]);
			}
			else
				shader->set_uniform("useSkinning", false);

			// Set the light properties
			shader->set_uniform("light.m_direction", scene.m_lightProperties.m_direction);
			shader->set_uniform("light.m_ambient", scene.m_lightProperties.m_ambient);
			shader->set_uniform("light.m_diffuse", scene.m_lightProperties.m_diffuse);
			shader->set_uniform("light.m_specular", scene.m_lightProperties.m_specular);

			// Set the material properties
			if (material.m_usesBaseTexture)
				shader->set_uniform("mat.m_diffuse", material.m_baseColorTex.get_texture_unit());
			else
				shader->set_uniform("mat.m_diffuse", glm::vec3(material.m_baseColor));

			if (material.m_usesNormalTexture && useNormalMap)
			{
				shader->set_uniform("camPosWorldSpace", scene.get_camera().get_position());
				shader->set_uniform("mat.m_normalMap", material.m_normalMapTex.get_texture_unit());
				shader->set_uniform("normalColorScale", material.m_normalMapScale);
			}

			shader->set_uniform("mat.m_shininess", 32.0f);
			
			// Call render on each primitive
			primitives[i].render();
		}
	}


	// Set the index of the referenced mesh inside the model's vector of meshes
	void MeshRenderable::set_mesh_idx(int meshIdx)
	{
		m_meshIdx = meshIdx;
	}

	// Get the index of the referenced mesh inside the model's vector of meshes
	int MeshRenderable::get_mesh_idx() const
	{
		return m_meshIdx;
	}

	// Set the bounding volume of this mesh as an aabb
	void MeshRenderable::set_local_bounding_volume(const AABB& newBv)
	{
		m_localBv = newBv;
	}

	// Get the bounding volume of this mesh as an aabb
	AABB MeshRenderable::get_local_bounding_volume() const
	{
		return m_localBv;
	}

	// Get the world bounding volume of this mesh as an aabb
	AABB MeshRenderable::get_world_bounding_volume() const
	{
		// Compute all the corners of the aabb in local space
		const glm::vec3& localDiagonal = m_localBv.m_max - m_localBv.m_min;
		glm::vec3 localBvCorners[8];
		localBvCorners[0] = m_localBv.m_min;
		localBvCorners[1] = m_localBv.m_min + glm::vec3(localDiagonal.x, 0.0f, 0.0f);
		localBvCorners[2] = m_localBv.m_min + glm::vec3(0.0f, localDiagonal.y, 0.0f);
		localBvCorners[3] = m_localBv.m_min + glm::vec3(localDiagonal.x, localDiagonal.y, 0.0f);
		localBvCorners[4] = m_localBv.m_min + glm::vec3(0.0f, 0.0f, localDiagonal.z);
		localBvCorners[5] = m_localBv.m_min + glm::vec3(localDiagonal.x, 0.0f, localDiagonal.z);
		localBvCorners[6] = m_localBv.m_min + glm::vec3(0.0f, localDiagonal.y, localDiagonal.z);
		localBvCorners[7] = m_localBv.m_max;


		// Get all the corners of the aabb in world space
		const glm::mat4& modelMtx = get_owner()->m_worldTr.get_model_mtx();
		glm::vec3 worldBvCorners[8];
		for (int i = 0; i < 8; ++i)
			worldBvCorners[i] = glm::vec3(modelMtx * glm::vec4(localBvCorners[i], 1.0f));


		// Make the bounding volume axis aligned again
		glm::vec3 finalMin{FLT_MAX, FLT_MAX, FLT_MAX };
		glm::vec3 finalMax{-FLT_MAX, -FLT_MAX, -FLT_MAX };
		for (int i = 0; i < 8; ++i)
		{
			if (worldBvCorners[i].x < finalMin.x)
				finalMin.x = worldBvCorners[i].x;
			else if (worldBvCorners[i].x > finalMax.x)
				finalMax.x = worldBvCorners[i].x;
		
			if (worldBvCorners[i].y < finalMin.y)
				finalMin.y = worldBvCorners[i].y;
			else if (worldBvCorners[i].y > finalMax.y)
				finalMax.y = worldBvCorners[i].y;
		
			if (worldBvCorners[i].z < finalMin.z)
				finalMin.z = worldBvCorners[i].z;
			else if (worldBvCorners[i].z > finalMax.z)
				finalMax.z = worldBvCorners[i].z;
		}

		return { finalMin, finalMax };
	}


	// Get wether the bounding volume of this mesh is being rendered
	bool MeshRenderable::get_draw_bounding_volume() const
	{
		return m_drawBv;
	}


	void MeshRenderable::on_gui()
	{
		Model* modelResource = get_owner()->get_model();
		if (modelResource)
		{
			ImGui::Text("Mesh Index: %i", m_meshIdx);

			Mesh& mesh = modelResource->m_meshes[m_meshIdx];
			ImGui::Text("Primitive Count: %i", mesh.m_primitives.size());

			ImGui::Checkbox("Draw Bounding Volume", &m_drawBv);
		}
	}
}