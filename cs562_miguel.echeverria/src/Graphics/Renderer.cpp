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
#include "Skybox.h"
#include "Resources/ResourceManager.h"
#include <GL/glew.h>


namespace cs460
{
	Renderer::Renderer()
		:	m_skybox(nullptr)
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
		// Draw each renderable
		for (int i = 0; i < m_renderables.size(); ++i)
		{
			// Skip drawing the mesh if it is not active
			if (!m_renderables[i]->get_active())
				continue;

			// Skip drawing the mesh if its entire model is not active
			ModelInstance* modelInst = m_renderables[i]->get_model_root_node()->get_component<ModelInstance>();
			if (modelInst == nullptr || !modelInst->get_active())	// TODO: modelInst == nullptr is a very temporary fix so that it doesn't crash when deleting a ModelInstance component
				continue;

			// Render all the primitives in the current mesh
			m_renderables[i]->render_primitives();
		}

		// Draw the skybox last if active
		m_skybox->render();
	}

	void Renderer::close()
	{
	}

	void Renderer::set_viewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
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


	// Change the skybox resource that will be rendered. Return true
	// if the skybox was found in the resource manager, false otherwise.
	bool Renderer::change_skybox(const std::string& skyboxIdName)
	{
		Skybox* newSkybox = ResourceManager::get_instance().get_skybox(skyboxIdName);

		if (newSkybox == nullptr)
		{
			std::cout << "WARNING: There isn't any skybox with \"" << skyboxIdName << "\" as key\n";
			return false;
		}

		m_skybox = newSkybox;
		return true;
	}


	Window& Renderer::get_window()
	{
		return m_window;
	}

	Skybox* Renderer::get_skybox()
	{
		return m_skybox;
	}

	void Renderer::set_gl_properties()
	{
		m_window.set_swap_interval(1);				// Enable v-sync
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glDisable(GL_CULL_FACE);
		
		//std::cout << glGetString(GL_VERSION) << std::endl;
	}
}