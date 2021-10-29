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
#include "Composition/Scene.h"
#include "Composition/SceneNode.h"
#include "Components/ModelInstance.h"
#include "Math/Interpolation/InterpolationFunctions.h"
#include "Platform/FrameRateController.h"
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
			// Go to the next if there is no property to update
			if (m_animProperties[i].m_property == nullptr)
				continue;

			// Get the animation data and number of components in this property
			int animIdx = m_animProperties[i].m_animIdx;
			int dataIdx = m_animProperties[i].m_animDataIdx;
			AnimationData& data = model->m_animations[animIdx].m_animData[dataIdx];
			int componentCount = data.m_componentCount;
			

			// Interpolate the current property based on the animation timer and the interpolation method
			if (m_animProperties[i].m_interpolationMode == INTERPOLATION_MODE::LERP)
			{
				const glm::vec3& interpolatedVal = piecewise_lerp(data.m_keys, data.m_values, m_animTimer);
				std::memcpy(m_animProperties[i].m_property, glm::value_ptr(interpolatedVal), 3 * sizeof(float));
			}
			else if (m_animProperties[i].m_interpolationMode == INTERPOLATION_MODE::SLERP)
			{
				const glm::quat& interpolatedVal = piecewise_slerp(data.m_keys, data.m_values, m_animTimer);
				std::memcpy(m_animProperties[i].m_property, glm::value_ptr(interpolatedVal), 4 * sizeof(float));
			}
			if (m_animProperties[i].m_interpolationMode == INTERPOLATION_MODE::STEP)
			{
				const glm::vec3& interpolatedVal = piecewise_step(data.m_keys, data.m_values, m_animTimer);
				std::memcpy(m_animProperties[i].m_property, glm::value_ptr(interpolatedVal), 3 * sizeof(float));
			}
			if (m_animProperties[i].m_interpolationMode == INTERPOLATION_MODE::CUBIC_SPLINE)
			{
				const glm::vec3& interpolatedVal = piecewise_hermite(data.m_keys, data.m_values, m_animTimer);
				std::memcpy(m_animProperties[i].m_property, glm::value_ptr(interpolatedVal), 3 * sizeof(float));
			}
		}


		// Update the timer of the animation
		m_animTimer += FrameRateController::get_instance().get_dt_float() * m_timeScale;

		// If looping and animation has finished, restart the animation
		if (m_looping && m_animTimer > m_duration)
			m_animTimer = 0.0f;
	}


	void AnimationReference::on_gui()
	{
		Model* model = get_owner()->get_model();

		if (ImGui::BeginCombo("Animation", m_previewName.c_str()))
		{
			if (ImGui::Selectable("None"))
				change_animation(-1, "None");

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
		ImGui::SliderFloat("Time Scale", &m_timeScale, 0.01f, 5.0f, "%.2f");
	}


	// Setter and getter for the index of the animation
	void AnimationReference::change_animation(int idx, const std::string& animName)
	{
		Scene& scene = Scene::get_instance();
		auto& modelNodes = scene.get_model_inst_nodes(get_owner()->get_model_root_node()->get_component<ModelInstance>()->get_instance_id());

		m_animIdx = idx;
		m_previewName = animName;

		// Nothing else to do if "no animation" option has been selected
		if (m_animIdx < 0)
			return;

		Model* model = get_owner()->get_model();
		Animation& anim = model->m_animations[m_animIdx];


		// Reset the timer of the animation and total duration
		m_animTimer = 0.0f;
		m_duration = anim.m_duration;


		// Initialize animation properties
		m_animProperties.clear();
		m_animProperties.resize(anim.m_channels.size());

		for (int i = 0; i < m_animProperties.size(); ++i)
		{
			// We will not be doing anything with weights at the moment
			if (anim.m_channels[i].m_targetProperty == "weights")
				continue;

			// Set the animation and anim data (sampler) indices
			m_animProperties[i].m_animIdx = m_animIdx;
			m_animProperties[i].m_animDataIdx = anim.m_channels[i].m_animDataIdx;
			
			// Get the target node, and set the property to the appropriate data
			SceneNode* targetNode = modelNodes[anim.m_channels[i].m_targetNodeIdx];
			if (anim.m_channels[i].m_targetProperty == "translation")
				m_animProperties[i].m_property = glm::value_ptr(targetNode->m_localTr.m_position);
			else if (anim.m_channels[i].m_targetProperty == "rotation")
				m_animProperties[i].m_property = glm::value_ptr(targetNode->m_localTr.m_orientation);
			else if (anim.m_channels[i].m_targetProperty == "scale")
				m_animProperties[i].m_property = glm::value_ptr(targetNode->m_localTr.m_scale);


			// Set the interpolation function
			const std::string& interpolationMethod = anim.m_animData[anim.m_channels[i].m_animDataIdx].m_interpolationMethod;
			if (interpolationMethod == "LINEAR")
			{
				m_animProperties[i].m_interpolationMode = INTERPOLATION_MODE::LERP;
				
				if (anim.m_channels[i].m_targetProperty == "rotation")
					m_animProperties[i].m_interpolationMode = INTERPOLATION_MODE::SLERP;
			}
			else if (interpolationMethod == "STEP")
				m_animProperties[i].m_interpolationMode = INTERPOLATION_MODE::STEP;
			else if (interpolationMethod == "CUBICSPLINE")
				m_animProperties[i].m_interpolationMode = INTERPOLATION_MODE::CUBIC_SPLINE;
		}
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