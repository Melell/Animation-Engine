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


namespace cs460
{
	class AnimationReference : public IComponent
	{
	public:

		AnimationReference();
		virtual ~AnimationReference();

		// Animate the properties of this animation
		void update_properties();

		// Setter and getter for the index of the animation
		void change_animation(int idx, const std::string& animName);
		int get_anim_idx() const;
		std::string get_anim_name() const;

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


		void on_gui() override;
	};
}