/**
* @file IComponent.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Base class from which all components inherit.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class SceneNode;


	class IComponent
	{
	public:

		virtual ~IComponent();

		void show_gui();	// Calls a private virtual ongui so that the ongui of the actual component gets called

		// Getter and setter for the scene node that owns this component
		void set_owner(SceneNode* newOwner);
		SceneNode* get_owner() const;

		// Getter and setter for the active bool which activates/deactivates the component's logic
		void set_active(bool newActive);
		bool get_active() const;

	private:

		SceneNode* m_owner = nullptr;
		bool m_active = true;

		virtual void on_gui();	// To be overriden by specific components
	};
}