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
#include "Components/Models/ModelInstance.h"
#include "Math/Interpolation/InterpolationFunctions.h"
#include "Platform/FrameRateController.h"
#include "Animation/Blending/BlendingCore.h"
#include "Animation/Blending/Blend1D.h"


namespace cs460
{
	AnimationReference::AnimationReference()
	{
		Animator::get_instance().add_animation_ref(this);
	}

	AnimationReference::~AnimationReference()
	{
		//std::cout << "ANIM REF DESTRUCTOR CALLED\n";
		Animator::get_instance().remove_animation_ref(this);
	}


	void AnimationReference::update()
	{
		// If using a blend tree, ignore the rest of the logic
		if (m_blendTreeType > 0)
		{
			if (m_blendTree == nullptr)
				return;

			m_blendTree->produce_pose(m_animTimer);
			apply_pose_to_skeleton(m_blendTree->m_pose, this);
		}
		else
		{
			// If no animation selected, or animation paused, don't update
			if (m_animIdx < 0 || m_paused)
				return;

			// Update the properties of the animation
			update_properties();
		}
		
		// Update the timer of the animation
		m_animTimer += FrameRateController::get_instance().get_dt_float() * m_timeScale;

		// Don't cap the timer when using blend trees
		if (m_blendTreeType > 0)
			return;

		// If looping and animation has finished, restart the animation
		if (m_looping && m_animTimer > m_duration)
			m_animTimer = 0.0f;
	}

	void AnimationReference::update_properties()
	{
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
	}


	void AnimationReference::on_gui()
	{
		Model* model = get_owner()->get_model();

		ImGui::Text("Blending options:");
		ImGui::RadioButton("None", &m_blendTreeType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("1D Blend", &m_blendTreeType, 1);
		ImGui::SameLine();
		ImGui::RadioButton("2D Blend", &m_blendTreeType, 2);

		if (m_blendTreeType == 1)
			blend_1d_editor();
		else if (m_blendTreeType)
			blend_2d_editor();

		if (m_blendTreeType > 0)
			return;

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


	void AnimationReference::blend_1d_editor()
	{
		// Create a subregion inside the window for drawing the editor
		if (!ImGui::BeginChild("1D Blending Editor", ImVec2(-1.0f, 60.0f), true))
		{
			ImGui::EndChild();
			return;
		}
		ImGui::EndChild();

		// Get the draw list for custom drawing
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Draw the background and define the clipping region for the editor
		const ImVec2& spaceMin = ImGui::GetItemRectMin();
		const ImVec2& spaceMax = ImGui::GetItemRectMax();
		const ImVec2& spaceSize = ImVec2(spaceMax.x - spaceMin.x, spaceMax.y - spaceMin.y);
		drawList->PushClipRect(spaceMin, spaceMax);
		drawList->AddRectFilled(spaceMin, spaceMax, IM_COL32(120, 120, 120, 120), 5.0f);

		
		// Draw the blend 1D blend space line
		ImVec2 lineStart(spaceMin.x + 0.025f * spaceSize.x, spaceMin.y + 0.5f * spaceSize.y);
		ImVec2 lineEnd(spaceMax.x - 0.025f * spaceSize.x, lineStart.y);
		float lineLength = lineEnd.x - lineStart.x;
		drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 200), 2.0f);

		// Sort the blend nodes first, to know the length of the blendspace
		Blend1D* blendTree = dynamic_cast<Blend1D*>(m_blendTree);
		blendTree->sort_children();

		IBlendNode* first = blendTree->m_children.front();
		IBlendNode* last = blendTree->m_children.back();
		float totalLength = last->m_blendPos.x - first->m_blendPos.x;

		// Then draw the nodes in the line
		for (IBlendNode* child : blendTree->m_children)
		{
			float currLength = child->m_blendPos.x - first->m_blendPos.x;
			float factor = currLength / totalLength;

			ImVec2 pos(lineStart.x + factor * lineLength, lineEnd.y);
			const float NODE_SCALE = 12.0f;
			float halfScale = 0.5f * NODE_SCALE;
			ImVec2 rectMin(pos.x - halfScale, pos.y - halfScale);
			ImVec2 rectMax(pos.x + halfScale, pos.y + halfScale);

			drawList->AddRectFilled(rectMin, rectMax, IM_COL32(0, 170, 205, 210), 3.0f);
		}

		// Draw the blend parameter
		float currLength = blendTree->m_blendParam - first->m_blendPos.x;
		float factor = currLength / totalLength;

		ImVec2 pos(lineStart.x + factor * lineLength, lineEnd.y);
		const float PARAM_NODE_SCALE = 8.0f;
		float halfScale = 0.5f * PARAM_NODE_SCALE;
		ImVec2 rectMin(pos.x - halfScale, pos.y - halfScale);
		ImVec2 rectMax(pos.x + halfScale, pos.y + halfScale);
		drawList->AddRectFilled(rectMin, rectMax, IM_COL32(0, 230, 80, 255), 3.0f);

		
		drawList->PopClipRect();
	}

	void AnimationReference::blend_2d_editor()
	{

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


	// Useful getters and setters
	float AnimationReference::get_anim_timer() const
	{
		return m_animTimer;
	}
	float AnimationReference::get_anim_duration() const
	{
		return m_duration;
	}
	float AnimationReference::get_anim_time_scale() const
	{
		return m_timeScale;
	}
	bool AnimationReference::get_anim_looping() const
	{
		return m_looping;
	}
	bool AnimationReference::get_anim_paused() const
	{
		return m_paused;
	}

	void AnimationReference::set_anim_time_scale(float newTimeScale)
	{
		m_timeScale = newTimeScale;
	}
	void AnimationReference::set_anim_looping(bool isLooping)
	{
		m_looping = isLooping;
	}
	void AnimationReference::set_anim_paused(bool isPaused)
	{
		m_paused = isPaused;
	}


	// Getter and setter for the type of blend tree to use (0=None, 1=1D, 2=2D)
	int AnimationReference::get_blend_tree_type() const
	{
		return m_blendTreeType;
	}

	void AnimationReference::set_blend_tree_type(int type)
	{
		m_blendTreeType = type;
	}
}