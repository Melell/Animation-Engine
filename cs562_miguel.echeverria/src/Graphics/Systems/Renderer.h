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
	class Skybox;


	class Renderer
	{
	public:
	
		~Renderer();

		static Renderer& get_instance();

		bool initialize();
		void render();
		void close();

		void set_viewport(int x, int y, int width, int height);

		// Clear the frame buffer color and depth
		void clear_fb();
		
		void add_mesh_renderable(MeshRenderable* renderable);		// Adds a mesh renderable component to the internal vector, so that it can be rendered
		void remove_mesh_renderable(MeshRenderable* renderable);	// Removes a mesh renderable component from the internal vector that is being rendered

		// Change the skybox resource that will be rendered. Return true
		// if the skybox was found in the resource manager, false otherwise.
		bool change_skybox(const std::string& skyboxIdName);

		Window& get_window();
		Skybox* get_skybox();

		// Debug draws the bounding volumes of the meshrenderables in the scene
		void debug_draw_bvs() const;

		int m_bvDrawMode = 2;	// 0 = Draw none. 1 = Draw the bv of the selected node. 2 = Draw all the bvs

	private:

		Window m_window;
		std::vector<MeshRenderable*> m_renderables;		// All the components that reference a mesh in a model
		Skybox* m_skybox;

		void set_gl_properties();

		// For singleton pattern
		Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
	};
}