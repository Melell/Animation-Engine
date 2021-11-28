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
#include "Animation/Blending/Blend2D.h"


namespace cs460
{
	AnimationReference::AnimationReference()
	{
		Animator::get_instance().add_animation_ref(this);

		m_1dBlendTree = new Blend1D;
		m_2dBlendTree = new Blend2D;
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
			// Sanity checks
			if (m_blendTreeType == 1 && m_1dBlendTree == nullptr)
				return;
			if (m_blendTreeType == 2 && m_2dBlendTree == nullptr)
				return;

			get_blend_tree()->produce_pose(m_animTimer);
			apply_pose_to_skeleton(get_blend_tree()->m_pose, this);
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
		else if (m_blendTreeType == 2)
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
		const ImVec2& editorMin = ImGui::GetItemRectMin();
		const ImVec2& editorMax = ImGui::GetItemRectMax();
		const ImVec2& editorSize = ImVec2(editorMax.x - editorMin.x, editorMax.y - editorMin.y);
		drawList->PushClipRect(editorMin, editorMax);
		drawList->AddRectFilled(editorMin, editorMax, IM_COL32(120, 120, 120, 120), 5.0f);

		
		// Draw the blend 1D blend space line
		ImVec2 lineStart(editorMin.x + 0.025f * editorSize.x, editorMin.y + 0.5f * editorSize.y);
		ImVec2 lineEnd(editorMax.x - 0.025f * editorSize.x, lineStart.y);
		float lineLength = lineEnd.x - lineStart.x;
		drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 200), 2.0f);

		// Sort the blend nodes first, to know the length of the blendspace
		Blend1D* blendTree = dynamic_cast<Blend1D*>(get_blend_tree());
		if (blendTree->m_children.empty())
		{
			drawList->PopClipRect();
			return;
		}
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
		// Create a subregion inside the window for drawing the editor
		float size = ImGui::GetWindowWidth() * 0.5f;
		if (!ImGui::BeginChild("2D Blending Editor", ImVec2(size, size), true))
		{
			ImGui::EndChild();
			return;
		}
		ImGui::EndChild();

		// Get the draw list for custom drawing
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Draw the background and define the clipping region for the editor
		const ImVec2& editorMin = ImGui::GetItemRectMin();
		const ImVec2& editorMax = ImGui::GetItemRectMax();
		const ImVec2& editorSize = ImVec2(editorMax.x - editorMin.x, editorMax.y - editorMin.y);
		drawList->PushClipRect(editorMin, editorMax);
		drawList->AddRectFilled(editorMin, editorMax, IM_COL32(120, 120, 120, 120), 5.0f);


		glm::vec2 editorWorkStart(editorMin.x + 0.025f * editorSize.x, editorMin.y + 0.025f * editorSize.y);
		glm::vec2 editorWorkEnd(editorMax.x - 0.025f * editorSize.x, editorMax.y - 0.025f * editorSize.y);
		glm::vec2 editorWorkSize = editorWorkEnd - editorWorkStart;

		// Get the boundaries of the blend space
		Blend2D* blendTree = dynamic_cast<Blend2D*>(get_blend_tree());
		const glm::vec2& minPos = blendTree->get_min_pos();
		const glm::vec2& maxPos = blendTree->get_max_pos();
		float xLength = maxPos.x - minPos.x;
		float yLength = maxPos.y - minPos.y;

		// Draw the triangles
		for (int i = 0; i < blendTree->m_triangles.size(); ++i)
		{
			const auto& indices = blendTree->m_triangles[i];
			const glm::vec2& v0 = blendTree->m_children[indices[0]]->m_blendPos;
			const glm::vec2& v1 = blendTree->m_children[indices[1]]->m_blendPos;
			const glm::vec2& v2 = blendTree->m_children[indices[2]]->m_blendPos;

			// Compute the normalized position of v0
			float currX0Length = v0.x - minPos.x;
			float x0Factor = currX0Length / xLength;
			float currY0Length = v0.y - minPos.y;
			float y0Factor = currY0Length / yLength;

			// Compute the normalized position of v1
			float currX1Length = v1.x - minPos.x;
			float x1Factor = currX1Length / xLength;
			float currY1Length = v1.y - minPos.y;
			float y1Factor = currY1Length / yLength;

			// Compute the normalized position of v2
			float currX2Length = v2.x - minPos.x;
			float x2Factor = currX2Length / xLength;
			float currY2Length = v2.y - minPos.y;
			float y2Factor = currY2Length / yLength;

			//if (glm::epsilonEqual(x0Factor, 0.0f, FLT_EPSILON) && glm::epsilonEqual(y0Factor, 0.0f, FLT_EPSILON))
			//	__debugbreak();
			//if (glm::epsilonEqual(x1Factor, 0.0f, FLT_EPSILON) && glm::epsilonEqual(y1Factor, 0.0f, FLT_EPSILON))
			//	__debugbreak();
			//if (glm::epsilonEqual(x2Factor, 0.0f, FLT_EPSILON) && glm::epsilonEqual(y2Factor, 0.0f, FLT_EPSILON))
			//	__debugbreak();
			
			// Draw v0 to v1
			ImVec2 v0v1Start(editorWorkStart.x + x0Factor * editorWorkSize.x, editorWorkStart.y + y0Factor * editorWorkSize.y);
			ImVec2 v0v1End(editorWorkStart.x + x1Factor * editorWorkSize.x, editorWorkStart.y + y1Factor * editorWorkSize.y);
			drawList->AddLine(v0v1Start, v0v1End, IM_COL32(255, 255, 255, 200), 2.0f);

			// Draw v1 to v2
			ImVec2 v1v2Start(editorWorkStart.x + x1Factor * editorWorkSize.x, editorWorkStart.y + y1Factor * editorWorkSize.y);
			ImVec2 v1v2End(editorWorkStart.x + x2Factor * editorWorkSize.x, editorWorkStart.y + y2Factor * editorWorkSize.y);
			drawList->AddLine(v1v2Start, v1v2End, IM_COL32(255, 255, 255, 200), 2.0f);

			// Draw v2 to v0
			ImVec2 v2v0Start(editorWorkStart.x + x2Factor * editorWorkSize.x, editorWorkStart.y + y2Factor * editorWorkSize.y);
			ImVec2 v2v0End(editorWorkStart.x + x0Factor * editorWorkSize.x, editorWorkStart.y + y0Factor * editorWorkSize.y);
			drawList->AddLine(v2v0Start, v2v0End, IM_COL32(255, 255, 255, 200), 2.0f);
		}


		drawList->PopClipRect();
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

	// Get the current blend tree (null, blend1d, or blend2d)
	IBlendNode* AnimationReference::get_blend_tree()
	{
		if (m_blendTreeType == 1)
			return m_1dBlendTree;
		else if (m_blendTreeType == 2)
			return m_2dBlendTree;

		return nullptr;
	}


	// TODO: Remove these in the future. This is just to be able to harcode the demos
	//void AnimationReference::set_1d_blend_tree(Blend1D* blendTree)
	//{
	//	m_1dBlendTree = blendTree;
	//}
	//
	//void AnimationReference::set_2d_blend_tree(Blend2D* blendTree)
	//{
	//	m_2dBlendTree = blendTree;
	//}
}