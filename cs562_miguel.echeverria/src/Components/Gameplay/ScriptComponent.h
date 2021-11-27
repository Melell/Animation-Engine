/**
* @file ScriptComponent.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Base component for all specific components that run some logic on its owner.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"


namespace cs460
{
	class ScriptComponent : public IComponent
	{
	public:

		ScriptComponent();
		virtual ~ScriptComponent();

		virtual void initialize() = 0;
		virtual void update() = 0;

	private:
		virtual void on_gui() override;
	};
}