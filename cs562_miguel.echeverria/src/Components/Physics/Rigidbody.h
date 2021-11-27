/**
* @file PlayerController.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Script that handles the movement of the player used for the animation
*		 blending demo.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"


namespace cs460
{
	class Rigidbody : public IComponent
	{
	public:

		Rigidbody();
		virtual ~Rigidbody();

	private:

		void on_gui() override;
	};
}