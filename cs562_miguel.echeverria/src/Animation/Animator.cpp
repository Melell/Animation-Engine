/**
* @file Animator.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents the animation system, in charge of animating joints
*		 of a skin, updating the joint matrices etc.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Animator.h"
#include "Components/SkinReference.h"
#include "Composition/SceneNode.h"
#include "Components/ModelInstance.h"
#include "Composition/Scene.h"
#include "Graphics/GLTF/Model.h"


namespace cs460
{
	Animator& Animator::get_instance()
	{
		static Animator instance;
		return instance;
	}

	Animator::Animator()
	{
	}

	Animator::~Animator()
	{
	}


	// System management functions
	bool Animator::initialize()
	{
		return true;
	}

	void Animator::update()
	{
		// Update animations...


		// Update the joint matrices of each skin
		update_skins();
	}

	void Animator::close()
	{

	}


	// Adds a skin root component to the internal vector
	void Animator::add_skin_root(SkinReference* rootComp)
	{
		m_skinReferences.push_back(rootComp);
	}

	// Removes a skin root component from the internal vector
	void Animator::remove_skin_root(SkinReference* rootComp)
	{
		auto foundIt = std::find(m_skinReferences.begin(), m_skinReferences.end(), rootComp);

		// Nothing to remove if not in the vector
		if (foundIt == m_skinReferences.end())
			return;

		m_skinReferences.erase(foundIt);
	}


	// Update the joint matrices of each skin
	void Animator::update_skins()
	{
		Scene& scene = Scene::get_instance();

		for (int i = 0; i < m_skinReferences.size(); ++i)
		{
			SkinReference* skinRef = m_skinReferences[i];
			ModelInstance* rootModelInst = skinRef->get_owner()->get_model_root_node()->get_component<ModelInstance>();
			int modelInstanceId = rootModelInst->get_instance_id();
			int skinIdx = skinRef->get_skin_idx();

			// Get the model resource, the skin, and the "dictionary" of nodes of this model instance
			Model* modelResource = skinRef->get_owner()->get_model();
			Skin& skin = modelResource->m_skins[skinIdx];
			auto& modelInstanceNodes = scene.get_model_inst_nodes(modelInstanceId);


			// Get the skeleton root inverse model to world matrix
			SceneNode* skeletonRootNode = modelInstanceNodes[skin.m_commonRootIdx];
			const glm::mat4& rootInvModelMtx = skeletonRootNode->m_worldTr.get_inv_model_mtx();

			// For each joint, update its joint matrix
			std::vector<glm::mat4>& jointMatrices = skinRef->get_joint_matrices();
			for (int j = 0; j < jointMatrices.size(); ++j)
			{
				// Get the model to world matrix of the current joint
				SceneNode* jointNode = modelInstanceNodes[skin.m_joints[j]];
				const glm::mat4& jointModelMtx = jointNode->m_worldTr.get_model_mtx();

				// Update the joint matrix
				jointMatrices[j] = rootInvModelMtx * jointModelMtx * skin.m_invBindMatrices[j];
			}
		}
	}
}