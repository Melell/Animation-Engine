/**
* @file Joint.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that represents a joint in a skin.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Joint.h"
#include "Composition/SceneNode.h"
#include "Graphics/GLTF/Model.h"
#include <imgui/imgui.h>


namespace cs460
{
	// Getters and setters for the skin index
	void Joint::set_skin_idx(int idx)
	{
		m_skinIdx = idx;
	}

	int Joint::get_skin_idx() const
	{
		return m_skinIdx;
	}


	void Joint::on_gui()
	{
		if (m_skinIdx >= 0)
		{
			Model* sourceModel = get_owner()->get_model();
			Skin& skin = sourceModel->m_skins[m_skinIdx];

			ImGui::Text("Owner Skin Index: %i", m_skinIdx);
			ImGui::Text("Skeleton Root Index: %i", skin.m_commonRootIdx);
		}
	}
}