/**
* @file MainMenuBarGUI.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief In charge of the GUI for the main menu toolbar at the top of the viewport.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class MainMenuBarGUI
	{
	public:

		friend class Editor;

		void update();

	private:

		MainMenuBarGUI();
	};
}