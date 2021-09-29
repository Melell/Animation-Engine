/**
* @file Renderer.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Singleton in charge of rendering the scene.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Platform/Window.h"


namespace cs460
{
	class MeshRenderable;


	class Renderer
	{
	public:
	
		~Renderer();

		static Renderer& get_instance();

		bool initialize();
		void render();
		void close();

		void clear_fb();		// Clear the frame buffer color and depth

		Window& get_window();

	private:

		std::vector<MeshRenderable*> m_renderables;		// All the components that reference a mesh in a model
		Window m_window;

		void set_gl_properties();

		// For singleton pattern
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
	};
}