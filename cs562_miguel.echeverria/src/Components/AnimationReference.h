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

#include "IComponent.h"
#include "Animation/Animation.h"
#include "Animation/Blending/BlendTree.h"


namespace cs460
{
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

		// Getter and setter for whether we are using the blend tree
		bool get_use_blend_tree() const;
		void set_use_blend_tree(bool useBlendTree);

		// Get the blend tree itself
		IBlendNode* m_blendTree;
		//IBlendNode*& get_blend_tree();
		//BlendTree& get_blend_tree();

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

		// Blend tree
		//BlendTree m_blendTree;
		bool m_useBlendTree = false;

		void on_gui() override;
	};
}