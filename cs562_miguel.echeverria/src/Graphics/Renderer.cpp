/**
* @file Renderer.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Implementation of the class that is in charge of rendering the scene.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Renderer.h"
#include "Components/MeshRenderable.h"
#include "Graphics/Model.h"
#include "Composition/SceneNode.h"
#include "Components/ModelInstance.h"
#include <GL/glew.h>


namespace cs460
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	Renderer& Renderer::get_instance()
	{
		static Renderer instance;
		return instance;
	}


	bool Renderer::initialize()
	{
		// Initialize glfw, create the window and associate an opengl context to it
		if (!m_window.create())
			return false;

		// Load all the opengl functions using glew
		GLenum glewStatus = glewInit();
		if (glewStatus != GLEW_OK)
		{
			std::cout << "ERROR: GLEW initialization failed\n";
			return false;
		}

		// Enable opengl error callbacks and call some other opengl configuration functions (for example setting up vsync or depth testing)
		cs460::enable_gl_debug_callbacks();
		set_gl_properties();

		return true;
	}

	void Renderer::render()
	{
		for (int i = 0; i < m_renderables.size(); ++i)
		{
			// Skip drawing the mesh if it is not active
			if (!m_renderables[i]->get_active())
				continue;

			// Skip drawing the mesh if its entire model is not active
			ModelInstance* modelInst = m_renderables[i]->get_model_root_node()->get_component<ModelInstance>();
			if (!modelInst->get_active())
				continue;

			m_renderables[i]->render_primitives();
		}
	}

	void Renderer::close()
	{
		// Release all buffers???

		
	}

	// Clear the frame buffer color and depths
	void Renderer::clear_fb()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	// Adds a mesh renderable components to the internal vector, so that it can be rendered
	void Renderer::add_mesh_renderable(MeshRenderable* renderable)
	{
		//auto foundIt = std::find(m_renderables.begin(), m_renderables.end(), renderable);

		// Probably won't happen, but just in case, check if the renderable is already in the vector to not add it twice
		//if (foundIt != m_renderables.end())
		//	return;

		m_renderables.push_back(renderable);
	}

	// Removes a mesh renderable component from the internal vector that is being rendered
	void Renderer::remove_mesh_renderable(MeshRenderable* renderable)
	{
		auto foundIt = std::find(m_renderables.begin(), m_renderables.end(), renderable);

		// Nothing to remove if not in the vector
		if (foundIt == m_renderables.end())
			return;

		m_renderables.erase(foundIt);
	}


	Window& Renderer::get_window()
	{
		return m_window;
	}

	void Renderer::set_gl_properties()
	{
		m_window.set_swap_interval(1);				// Enable v-sync
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);

		//std::cout << glGetString(GL_VERSION) << std::endl;
	}
}