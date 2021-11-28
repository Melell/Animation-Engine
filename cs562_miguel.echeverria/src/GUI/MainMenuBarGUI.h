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
	enum class SCENE_TO_LOAD
	{
		NONE,
		EMPTY,
		LINEAR,
		HERMITE,
		CATMULL_ROM,
		BEZIER,
		SKINNED_ANIMATION,
		PATH_FOLLOWING,
		BLENDING_1D,
		BLENDING_2D
	};


	class MainMenuBarGUI
	{
	public:

		friend class Editor;

		void update();

		// TODO: Remove this when serialization is implemented
		static MainMenuBarGUI& get_main_menu_bar_gui();

		void load_scene();


	private:
		SCENE_TO_LOAD m_sceneToLoad = SCENE_TO_LOAD::NONE;

		void load_empty_scene();
		void load_linear_curve_scene();
		void load_hermite_curve_scene();
		void load_catmull_rom_curve_scene();
		void load_bezier_curve_scene();
		void load_skinned_animation_scene();
		void load_path_following_scene();
		void load_blending_1d_scene();
		void load_blending_2d_scene();

		MainMenuBarGUI();
		MainMenuBarGUI(const MainMenuBarGUI&) = delete;
		MainMenuBarGUI& operator=(const MainMenuBarGUI&) = delete;
	};
}