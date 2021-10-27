/**
* @file AnimationReference.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that references an animation resource (this is added
*		 to the node with the model instance component).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "AnimationReference.h"
#include "Animation/Animator.h"
#include "Composition/SceneNode.h"
#include "Graphics/GLTF/Model.h"
#include <imgui/imgui.h>


namespace cs460
{
	AnimationReference::AnimationReference()
	{
		Animator::get_instance().add_animation_ref(this);
	}

	AnimationReference::~AnimationReference()
	{
		Animator::get_instance().remove_animation_ref(this);
	}


	void AnimationReference::update_properties()
	{
		// If no animation selected, or animation paused, don't update
		if (m_animIdx < 0 || m_paused)
			return;

		Model* model = get_owner()->get_model();

		for (int i = 0; i < m_animProperties.size(); ++i)
		{
			int animIdx = m_animProperties[i].m_animIdx;
			int dataIdx = m_animProperties[i].m_animDataIdx;

			// TODO: This is probably wrong because animation values are not given by gltf in float arrays, but in vec3/vec4 arrays
			AnimationData& data = model->m_animations[animIdx].m_animData[dataIdx];
			float interpolatedVal = m_animProperties[i].m_interpolationFn(data.m_values.data(), data.m_keys.data(), data.m_keys.size(), m_animTimer);
			m_animProperties[i].m_property[0] = interpolatedVal;
		}
	}


	void AnimationReference::on_gui()
	{
		Model* model = get_owner()->get_model();

		if (ImGui::BeginCombo("Animation", m_previewName.c_str()))
		{
			for (int i = 0; i < model->m_animations.size(); i++)
			{
				if (ImGui::Selectable(model->m_animations[i].m_name.c_str()))
				{
					change_animation(i, model->m_animations[i].m_name);
				}
			}

			ImGui::EndCombo();
		}

		ImGui::Checkbox("Loop", &m_looping);
		ImGui::Checkbox("Paused", &m_paused);
	}


	// Setter and getter for the index of the animation
	void AnimationReference::change_animation(int idx, const std::string& animName)
	{
		m_animIdx = idx;
		m_previewName = animName;

		// Reset the timer of the animation
		m_animTimer = 0.0f;


		// TODO: Initialize animation properties

	}

	int AnimationReference::get_anim_idx() const
	{
		return m_animIdx;
	}

	std::string AnimationReference::get_anim_name() const
	{
		return m_previewName;
	}
}