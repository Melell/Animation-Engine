/**
* @file SkinReference.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that indicates this node's mesh is using a skin.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "SkinReference.h"
#include "Animation/Animator.h"
#include "Graphics/GLTF/Model.h"
#include "Composition/SceneNode.h"
#include <imgui/imgui.h>


namespace cs460
{
	SkinReference::SkinReference()
	{
		Animator::get_instance().add_skin_ref(this);
	}

	SkinReference::~SkinReference()
	{
		Animator::get_instance().remove_skin_ref(this);
	}


	// Getters and setters for the skin index
	void SkinReference::set_skin_idx(int idx)
	{
		m_skinIdx = idx;

		// Resize the joint matrices vector so that it has enough space for a matrix per joint
		Model* modelResource = get_owner()->get_model();
		m_jointMatrices.resize(modelResource->m_skins[idx].m_joints.size());
	}

	int SkinReference::get_skin_idx() const
	{
		return m_skinIdx;
	}

	std::vector<glm::mat4>& SkinReference::get_joint_matrices()
	{
		return m_jointMatrices;
	}

	bool SkinReference::get_draw_skeleton() const
	{
		return m_drawSkeleton;
	}


	void SkinReference::on_gui()
	{
		Model* modelResource = get_owner()->get_model();
		if (modelResource)
		{
			ImGui::Text("Skin Index: %i", m_skinIdx);

			Skin& skin = modelResource->m_skins[m_skinIdx];
			ImGui::Text("Joint count: %i", skin.m_joints.size());

			ImGui::Checkbox("Draw Skeleton", &m_drawSkeleton);
		}
	}
}