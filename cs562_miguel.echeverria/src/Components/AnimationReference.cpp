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


	void AnimationReference::on_gui()
	{

	}
}