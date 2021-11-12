/**
* @file DebugRenderer.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief System that provides functions for debug drawing (very expensive, only for debugging).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "DebugRenderer.h"
#include "Math/Geometry/Geometry.h"
#include "Resources/ResourceManager.h"
#include "Graphics/Rendering/Shader.h"
#include "Composition/Scene.h"
#include "Animation/Animator.h"
#include "Components/SkinReference.h"
#include "Composition/SceneNode.h"
#include "Graphics/GLTF/Model.h"
#include "Components/ModelInstance.h"
#include <GL/glew.h>


namespace cs460
{
	glm::vec4 DebugRenderer::s_bvsColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


	bool DebugRenderer::s_enableSkeletonDrawing = false;
	glm::vec4 DebugRenderer::s_boneColor{ 0.25f, 1.0f, 0.25f, 1.0f };
	glm::vec4 DebugRenderer::s_jointColor{ 0.8f, 0.4f, 0.0f, 1.0f };
	float DebugRenderer::s_jointSize = 0.025f;


	bool DebugRenderer::s_enableMovingObjectDrawing = false;
	glm::vec4 DebugRenderer::s_movingObjectColor{ 0.1f, 0.9f, 0.4f, 1.0f };
	float DebugRenderer::s_movingObjectSize = 0.4f;

	bool DebugRenderer::s_enableCurveDrawing = true;
	glm::vec4 DebugRenderer::s_curveColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 DebugRenderer::s_curvePointColor{ 1.0f, 0.4f, 0.0f, 1.0f };
	float DebugRenderer::s_curvePointSize = 0.2f;

	bool DebugRenderer::s_enableTangentDrawing = true;
	glm::vec4 DebugRenderer::s_tangentLineColor{ 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 DebugRenderer::s_tangentEndpointColor{ 0.0f, 1.0f, 0.0f, 1.0f };
	float DebugRenderer::s_tangentEndpointSize = 0.2f;

	bool DebugRenderer::s_enableTableSamplesDrawing = false;
	glm::vec4 DebugRenderer::s_tableSampleColor{ 0.012f, 0.678f, 0.988f, 1.0f };
	float DebugRenderer::s_tableSampleSize = 0.05f;


	// TODO: Make this more efficient by saving the meshes

	void DebugRenderer::draw_point(const glm::vec3& position, const glm::vec4& color, float pointSize)
	{
		// Shader and uniform setup
		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Shader* shader = resourceMgr.get_shader("simple");
		shader->use();
		shader->set_uniform("color", color);
		shader->set_uniform("modelToWorld", glm::mat4(1.0f));	// Identity matrix since point is already in world space

		Scene& scene = Scene::get_instance();
		EditorCamera& cam = scene.get_camera();
		shader->set_uniform("worldToView", cam.get_view_mtx());
		shader->set_uniform("perspectiveProj", cam.get_projection_mtx());

		// Buffer setup
		glBindVertexArray(0);

		unsigned vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(position), glm::value_ptr(position), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);

		// For different point sizes
		glPointSize(pointSize);

		// Draw the actual point
		glDrawArrays(GL_POINTS, 0, 1);

		// Buffer cleanup
		glBindVertexArray(0);
		glDeleteBuffers(1, &vbo);
	}

	void DebugRenderer::draw_segment(const Segment& segment, const glm::vec4& color)
	{
		// Shader and uniform setup
		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Shader* shader = resourceMgr.get_shader("simple");
		shader->use();
		shader->set_uniform("color", color);
		shader->set_uniform("modelToWorld", glm::mat4(1.0f));	// Identity matrix since segment is already in world space

		Scene& scene = Scene::get_instance();
		EditorCamera& cam = scene.get_camera();
		shader->set_uniform("worldToView", cam.get_view_mtx());
		shader->set_uniform("perspectiveProj", cam.get_projection_mtx());

		// Buffer setup
		glBindVertexArray(0);

		unsigned vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		float data[6] = { segment.m_start.x, segment.m_start.y, segment.m_start.z,
						  segment.m_end.x,   segment.m_end.y,   segment.m_end.z };
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);

		// Draw the segment
		glDrawArrays(GL_LINES, 0, 2);

		// Buffer cleanup
		glBindVertexArray(0);
		glDeleteBuffers(1, &vbo);
	}

	void DebugRenderer::draw_aabb(const AABB& aabb, const glm::vec4& color, bool wireframe)
	{
		// Shader and uniform setup
		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Shader* shader = resourceMgr.get_shader("simple");
		shader->use();
		shader->set_uniform("color", color);

		Scene& scene = Scene::get_instance();
		EditorCamera& cam = scene.get_camera();
		shader->set_uniform("worldToView", cam.get_view_mtx());
		shader->set_uniform("perspectiveProj", cam.get_projection_mtx());
		
		// Compute model to world from the given aabb data
		glm::vec3 diagonal = aabb.m_max - aabb.m_min;
		glm::vec3 pos = aabb.m_min + diagonal * 0.5f;

		glm::mat4 m2w = glm::translate(glm::mat4(1.0f), pos);
		m2w = glm::scale(m2w, diagonal);

		shader->set_uniform("modelToWorld", m2w);

		// Get the already created cube (range [-0.5f, 0.5f])
		Cube& cube = resourceMgr.get_cube();
		cube.bind();

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Draw the cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		cube.unbind();
	}


	void DebugRenderer::draw_aabb(const glm::mat4& m2w, const glm::vec4& color, bool wireframe)
	{
		// Shader and uniform setup
		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Shader* shader = resourceMgr.get_shader("simple");
		shader->use();
		shader->set_uniform("color", color);

		Scene& scene = Scene::get_instance();
		EditorCamera& cam = scene.get_camera();
		shader->set_uniform("worldToView", cam.get_view_mtx());
		shader->set_uniform("perspectiveProj", cam.get_projection_mtx());
		shader->set_uniform("modelToWorld", m2w);

		// Get the already created cube (range [-0.5, 0.5])
		Cube& cube = resourceMgr.get_cube();
		cube.bind();

		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Draw the cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		cube.unbind();
	}


	void DebugRenderer::draw_curve_node(const glm::vec3& worldPos, const glm::vec4& color, float size)
	{
		// Prepare the aabb of specific size
		AABB box;
		float halfSize = size * 0.5f;
		glm::vec3 halfDiagonal(halfSize, halfSize, halfSize);
		box.m_min = worldPos - halfDiagonal;
		box.m_max = worldPos + halfDiagonal;

		// Draw the point as an aabb
		DebugRenderer::draw_aabb(box, color);
	}


	void DebugRenderer::draw_all_skeletons(const glm::vec4& boneColor, const glm::vec4& jointColor, float jointSize)
	{
		if (!s_enableSkeletonDrawing)
			return;

		Animator& animator = Animator::get_instance();
		Scene& scene = Scene::get_instance();

		// Iterate through each skin reference component to draw the skins one by one
		for (int i = 0; i < animator.m_skinReferences.size(); ++i)
		{
			SkinReference* skinRef = animator.m_skinReferences[i];
			if (!skinRef->get_draw_skeleton())
				continue;

			// Get some necessary variables to later get the skin
			Model* sourceModel = skinRef->get_owner()->get_model();
			int skinIdx = skinRef->get_skin_idx();

			// Get the "dictionary" of nodes for the model of the current skin
			int modelInstanceId = skinRef->get_owner()->get_model_root_node()->get_component<ModelInstance>()->get_instance_id();
			auto& modelInstNodes = scene.get_model_inst_nodes(modelInstanceId);


			// If the root is part of the joints, draw it as joint and draw the link with its children
			Skin& skin = sourceModel->m_skins[skinIdx];
			if (std::find(skin.m_joints.begin(), skin.m_joints.end(), skin.m_commonRootIdx) != skin.m_joints.end())
			{
				draw_skeleton_hierarchy(boneColor, jointColor, jointSize, sourceModel, skin, modelInstNodes, skin.m_commonRootIdx);
			}
			// Otherwise, draw the skeleton hierarchies taking each  of the children of the root as the root of the hierarchy to draw
			else
			{
				GLTFNode& rootNode = sourceModel->m_nodes[skin.m_commonRootIdx];
				for (int j = 0; j < rootNode.m_childrenIndices.size(); ++j)
				{
					int childIdx = rootNode.m_childrenIndices[i];
					draw_skeleton_hierarchy(boneColor, jointColor, jointSize, sourceModel, skin, modelInstNodes, childIdx);
				}
			}
		}
	}


	void DebugRenderer::draw_joint(const glm::vec3& worldPos, const glm::vec4& color, float jointSize)
	{
		glm::vec3 diagonal{ jointSize, jointSize, jointSize };
		glm::vec3 halfDiagonal = 0.5f * diagonal;
		AABB jointBox;
		jointBox.m_min = worldPos - halfDiagonal;
		jointBox.m_max = worldPos + halfDiagonal;

		DebugRenderer::draw_aabb(jointBox, color, false);
	}


	void DebugRenderer::draw_skeleton_hierarchy(const glm::vec4& boneColor, const glm::vec4& jointColor, float jointSize, Model* sourceModel, const Skin& skin, std::unordered_map<int, SceneNode*>& modelInstNodes, int rootIdx)
	{
		// Draw the joint of the root
		SceneNode* rootNode = modelInstNodes[rootIdx];
		draw_joint(rootNode->m_worldTr.m_position, jointColor, jointSize);

		// Draw the hierarchy of each of the children
		GLTFNode& gltfNode = sourceModel->m_nodes[rootIdx];
		for (int i = 0; i < gltfNode.m_childrenIndices.size(); ++i)
		{
			int childIdx = gltfNode.m_childrenIndices[i];
			SceneNode* childNode = modelInstNodes[childIdx];

			Segment seg;
			seg.m_start = rootNode->m_worldTr.m_position;
			seg.m_end = childNode->m_worldTr.m_position;

			draw_segment(seg, boneColor);
			draw_skeleton_hierarchy(boneColor, jointColor, jointSize, sourceModel, skin, modelInstNodes, gltfNode.m_childrenIndices[i]);
		}
	}
}