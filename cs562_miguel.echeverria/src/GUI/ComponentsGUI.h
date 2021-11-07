/**
* @file ComponentsGUI.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief In charge of the components editor window.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class ComponentsGUI
	{
	public:

		friend class Editor;

		void update();

	private:

		ComponentsGUI();
		ComponentsGUI(const ComponentsGUI&) = delete;
		ComponentsGUI& operator=(const ComponentsGUI&) = delete;
	};
}