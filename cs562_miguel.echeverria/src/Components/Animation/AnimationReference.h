/**
* @file AnimationReference.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that references an animation resource (this is added
*		 to the node with the model instance component).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"
#include "Animation/Animation.h"


namespace cs460
{
	struct IBlendNode;
	struct Blend1D;
	struct Blend2D;


	class AnimationReference : public IComponent
	{
	public:

		AnimationReference();
		virtual ~AnimationReference();

		// Update the animation
		void update();
		void update_properties();

		void change_animation(int idx, const std::string& animName);

		// Getters for the index of the animation and name
		int get_anim_idx() const;
		std::string get_anim_name() const;

		// Useful getters and setters
		float get_anim_timer() const;
		float get_anim_duration() const;
		float get_anim_time_scale() const;
		bool get_anim_looping() const;
		bool get_anim_paused() const;

		void set_anim_time_scale(float newTimeScale);
		void set_anim_looping(bool isLooping);
		void set_anim_paused(bool isPaused);

		// Getter and setter for the type of blend tree to use (0=None, 1=1D, 2=2D)
		int get_blend_tree_type() const;
		void set_blend_tree_type(int type);

		// Get the current blend tree (null, blend1d, or blend2d)
		IBlendNode* get_blend_tree();

		// TODO: Remove these in the future. This is just to be able to harcode the demos
		//void set_1d_blend_tree(Blend1D* blendTree);
		//void set_2d_blend_tree(Blend2D* blendTree);

	private:

		// Anim resource related data
		std::vector<AnimationProperty> m_animProperties;
		std::string m_previewName = "None";
		int m_animIdx = -1;

		// Animation control data
		float m_animTimer = 0.0f;
		float m_duration = 0.0f;
		float m_timeScale = 1.0f;
		bool m_looping = true;
		bool m_paused = false;

		// The 1d and 2d blending trees
		Blend1D* m_1dBlendTree = nullptr;
		Blend2D* m_2dBlendTree = nullptr;
		int m_blendTreeType = 0;

		// Blend tree gui params
		const glm::vec4 BACKGROUND_COLOR{ 120, 120, 120, 120 };
		const glm::vec4 LINES_COLOR{ 255, 255, 255, 200 };
		const glm::vec4 NODES_COLOR{ 0, 170, 205, 210 };
		const float NODE_SCALE = 12.0f;
		const glm::vec4 PARAM_NODE_COLOR{ 0, 230, 80, 255 };
		const float PARAM_NODE_SCALE = 8.0f;


		void on_gui() override;
		void blend_1d_editor();
		void blend_2d_editor();
		void blend_param_picking(const glm::vec2& windowCoordsStart, const glm::vec2& windowCoordsEnd, const ImVec2& rectMin, const ImVec2& rectMax, const glm::vec2& blendSpaceMin, const glm::vec2& blendSpaceMax, IBlendNode* blendTree);
	};
}