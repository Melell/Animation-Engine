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
#include "Components/AnimationReference.h"


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
		// Update all the animations' properties
		update_animations();

		// Update the joint matrices of each skin
		update_skins();
	}

	void Animator::close()
	{

	}


	// Adds an animation reference component to the internal vector
	void Animator::add_animation_ref(AnimationReference* animComp)
	{
		m_animReferences.push_back(animComp);
	}

	// Removes an animation reference component from the internal vector
	void Animator::remove_animation_ref(AnimationReference* animComp)
	{
		auto foundIt = std::find(m_animReferences.begin(), m_animReferences.end(), animComp);

		// Nothing to remove if not in the vector
		if (foundIt == m_animReferences.end())
			return;

		m_animReferences.erase(foundIt);
	}


	// Adds a skin reference component to the internal vector
	void Animator::add_skin_ref(SkinReference* skinComp)
	{
		m_skinReferences.push_back(skinComp);
	}

	// Removes a skin reference component from the internal vector
	void Animator::remove_skin_ref(SkinReference* skinComp)
	{
		auto foundIt = std::find(m_skinReferences.begin(), m_skinReferences.end(), skinComp);

		// Nothing to remove if not in the vector
		if (foundIt == m_skinReferences.end())
			return;

		m_skinReferences.erase(foundIt);
	}


	// Update each animation
	void Animator::update_animations()
	{
		for (int i = 0; i < m_animReferences.size(); ++i)
		{
			AnimationReference* animComp = m_animReferences[i];
			animComp->update();
		}
	}

	// Update the joint matrices of each skin
	void Animator::update_skins()
	{
		Scene& scene = Scene::get_instance();

		for (int i = 0; i < m_skinReferences.size(); ++i)
		{
			// TODO: Put all of this inside a function of SkinReference and make skinRef->get_joint_matrices function const

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
				jointMatrices[j] = skeletonRootNode->m_localTr.get_model_mtx() * rootInvModelMtx * jointModelMtx * skin.m_invBindMatrices[j];
			}
		}
	}
}