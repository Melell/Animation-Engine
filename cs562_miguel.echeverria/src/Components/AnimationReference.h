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


namespace cs460
{
	class AnimationReference : public IComponent
	{
	public:

		AnimationReference();
		virtual ~AnimationReference();

	private:
		void on_gui() override;
	};
}