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
	struct IKChain;


	enum class SCENE_TO_LOAD
	{
		NONE,
		EMPTY,
		LINEAR,
		HERMITE,
		CATMULL_ROM,
		BEZIER,
		SKINNED_ANIMATION,
		NESTED_MODELS,
		PATH_FOLLOWING,
		BLENDING_1D,
		BLENDING_2D,
		BLEND_EDITOR_1D,
		BLEND_EDITOR_2D,
		IK_ANALYTICAL_2D,
		IK_CCD_3D,
		IK_FABRIK_3D,
		IK_ON_SKELETON,
		CLOTH_SIMULATION,
		CLOTH_COLLISION
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
		void load_nested_models_scene();
		void load_path_following_scene();
		void load_blending_1d_scene();
		void load_blending_2d_scene();
		void load_blend_editor_1d_scene();
		void load_blend_editor_2d_scene();
		void load_ik_analytical_2d_scene();
		void load_ik_ccd_3d_scene();
		void load_ik_fabrik_3d_scene();
		void load_ik_on_skeleton_scene();
		void load_cloth_simulation_scene();
		void load_cloth_collision_scene();


		MainMenuBarGUI();
		MainMenuBarGUI(const MainMenuBarGUI&) = delete;
		MainMenuBarGUI& operator=(const MainMenuBarGUI&) = delete;
	};
}