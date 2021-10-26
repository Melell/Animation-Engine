/**
* @file Animator.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents the animation system, in charge of animating joints
*		 of a skin, updating the joint matrices etc.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class SkinReference;

	class Animator
	{
	public:

		friend class DebugRenderer;			// So that the debug renderer draw_all_skeleton function can access the skin references

		static Animator& get_instance();
		~Animator();

		// System management functions
		bool initialize();
		void update();
		void close();

		void add_skin_root(SkinReference* rootComp);		// Adds a skin root component to the internal vector
		void remove_skin_root(SkinReference* rootComp);		// Removes a skin root component from the internal vector

	private:

		std::vector<SkinReference*> m_skinReferences;

		Animator();
		Animator(const Animator&) = delete;
		Animator& operator=(const Animator&) = delete;


		// Update the joint matrices of each skin
		void update_skins();
	};
}