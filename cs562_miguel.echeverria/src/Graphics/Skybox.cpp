/**
* @file Skybox.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Cubemap that represents the sky..
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Skybox.h"
#include "Resources/ResourceManager.h"
#include "Graphics/Shader.h"
#include "Composition/Scene.h"
#include <GL/glew.h>


namespace cs460
{
	Skybox::Skybox()
		:	m_isActive(true)
	{
	}

	Skybox::~Skybox()
	{
	}

	// Render the skybox. Supposed to be the last thing rendered in the scene.
	void Skybox::render()
	{
		if (!m_isActive)
			return;

		ResourceManager& resourceManager = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		Cube& geometry = resourceManager.get_cube();
		Shader* shader = resourceManager.get_shader("skybox");

		glDepthFunc(GL_LEQUAL);

		// Bind the shader and set its uniforms
		shader->use();
		//shader->set_uniform("modelToWorld", glm::mat4(1.0f));
		shader->set_uniform("worldToView", glm::mat4(glm::mat3(scene.get_camera().get_view_mtx())));
		shader->set_uniform("perspectiveProj", scene.get_camera().get_projection_mtx());
		shader->set_uniform("skyboxSampler", m_cubeMap.get_texture_unit());

		// Bind the geometry data
		geometry.bind();

		// Bind the cubemap texture
		m_cubeMap.bind();

		// Draw the cube
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Unbind the geometry
		geometry.unbind();

		glDepthFunc(GL_LESS);
	}

	// Start/stop using the skybox (rendering it)
	void Skybox::set_active(bool newActive)
	{
		m_isActive = newActive;
	}
	bool Skybox::get_active()
	{
		return m_isActive;
	}

	// Loads the new skybox
	void Skybox::load_skybox(const std::string& newDirectoryPath)
	{
		// Delete the already existing cubemap
		//m_cubeMap.clear();
		m_cubeMap.set_texture_unit(0);

		m_cubeMap.bind();

		// Load the cubemap by specifying the 6 textures to use
		std::vector<std::string> filePaths(6, newDirectoryPath);
		try
		{
			fs::path startingPath(newDirectoryPath);

			if (!fs::exists(startingPath) || !fs::is_directory(startingPath))
			{
				std::cout << "ERROR: Starting path doesn't exist or is not a directory\n";
				return;
			}

			// Add all the front, back, left etc files into the filePaths vector so that they are loaded
			for (const auto& dir_it : fs::directory_iterator(startingPath))
			{
				if (dir_it.is_regular_file())
				{
					std::string fileName = dir_it.path().filename().generic_string();
					if (fileName.find("right") != fileName.npos)
						filePaths[0] += "/" + fileName;
					else if (fileName.find("left") != fileName.npos)
						filePaths[1] += "/" + fileName;
					else if (fileName.find("top") != fileName.npos)
						filePaths[2] += "/" + fileName;
					else if (fileName.find("bottom") != fileName.npos)
						filePaths[3] += "/" + fileName;
					else if (fileName.find("front") != fileName.npos)
						filePaths[4] += "/" + fileName;
					else if (fileName.find("back") != fileName.npos)
						filePaths[5] += "/" + fileName;
				}
			}
		}
		catch (const fs::filesystem_error& error)
		{
			std::cout << "FILESYSTEM ERROR: " << error.what() << std::endl;
			return;
		}

		m_cubeMap.load_cubemap_textures(filePaths);
		m_directory = newDirectoryPath;

		m_cubeMap.set_texture_parameters();

		m_cubeMap.unbind();
	}

	const std::string& Skybox::get_directory() const
	{
		return m_directory;
	}
}