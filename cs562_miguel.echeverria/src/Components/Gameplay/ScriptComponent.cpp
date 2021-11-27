/**
* @file LogicComponent.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Base component for all specific components that run some logic on its owner.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ScriptComponent.h"
#include "Gameplay/Systems/ScriptMgr.h"


namespace cs460
{
	ScriptComponent::ScriptComponent()
	{
		// Add to the script component manager
		ScriptMgr::get_instance().add_script(this);
	}

	ScriptComponent::~ScriptComponent()
	{
		// Remove from the script component manager
		ScriptMgr::get_instance().remove_script(this);
	}

	void ScriptComponent::initialize()
	{
	}

	void ScriptComponent::update()
	{
	}

	void ScriptComponent::on_gui()
	{
	}
}