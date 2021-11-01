/**
* @file CameraControlsGUI.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Logic of the imgui window that shows all the configurable camera parameters.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class CameraControlsGUI
	{
	public:

		friend class Editor;

		void update();

	private:

		CameraControlsGUI();
	};
}