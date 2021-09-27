/**
* @file IComponent.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Base class from which all components inherit.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "IComponent.h"
#include "Composition/SceneNode.h"
#include "Utilities/Rtti.h"
#include <imgui/imgui.h>


namespace cs460
{
	// Calls a private virtual ongui so that the ongui of the actual component gets called
	void IComponent::show_gui()
	{
		if (ImGui::CollapsingHeader(getTypeName(*this).erase(0, 7).c_str()))
		{
			bool deleteComp = false;

			if (ImGui::Button("Delete Component"))
				deleteComp = true;

			on_gui();

			// This should probably go in SceneNode on_gui function
			if (deleteComp)
				m_owner->delete_component(this);
		}
	}

	void IComponent::set_owner(SceneNode* newOwner)
	{
		m_owner = newOwner;
	}
	SceneNode* IComponent::get_owner() const
	{
		return m_owner;
	}

	void IComponent::set_active(bool newActive)
	{
		m_active = newActive;
	}
	bool IComponent::get_active() const
	{
		return m_active;
	}

	// To be overriden by specific components
	void IComponent::on_gui()
	{
	}
}