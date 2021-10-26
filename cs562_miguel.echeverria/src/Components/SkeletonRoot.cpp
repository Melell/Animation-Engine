/**
* @file SkeletonRoot.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that represents the common root of joints in a skin. Merely
*		 for displaying this information in the editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "SkeletonRoot.h"
#include "Composition/SceneNode.h"
#include "Graphics/GLTF/Model.h"
#include <imgui/imgui.h>


namespace cs460
{
	SkeletonRoot::SkeletonRoot()
	{
	}

	SkeletonRoot::~SkeletonRoot()
	{
	}


	// Getters and setters for the skin index
	void SkeletonRoot::set_skin_idx(int idx)
	{
		m_skinIdx = idx;
	}

	int SkeletonRoot::get_skin_idx() const
	{
		return m_skinIdx;
	}


	void SkeletonRoot::on_gui()
	{
		if (m_skinIdx >= 0)
		{
			Model* sourceModel = get_owner()->get_model();
			Skin& skin = sourceModel->m_skins[m_skinIdx];

			ImGui::Text("Owner Skin Index: %i", m_skinIdx);
			ImGui::Text("Joint Count: %i", skin.m_joints.size());
		}
	}
}