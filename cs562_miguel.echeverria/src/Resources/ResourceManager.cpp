/**
* @file ResourceManager.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Singleton that acts as a pool of resources (models, textures, etc).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ResourceManager.h"
#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/Skybox.h"


namespace cs460
{
	ResourceManager::ResourceManager()
	{

	}


	ResourceManager::~ResourceManager()
	{
		clear_resources();
	}


	ResourceManager& ResourceManager::get_instance()
	{
		static ResourceManager instance;
		return instance;
	}


	// Loads all the necessary resources for the demos. Meant to be called in Engine::Initialize.
	void ResourceManager::load_resources()
	{
		// Load all the shaders
		load_shader("simple", "src/Shaders/simple.vert", "src/Shaders/simple.frag");
		load_shader("phong_color", "src/Shaders/phong_color.vert", "src/Shaders/phong_color.frag");
		load_shader("phong_texture", "src/Shaders/phong_texture.vert", "src/Shaders/phong_texture.frag");
		load_shader("phong_normal_map", "src/Shaders/phong_normal_map.vert", "src/Shaders/phong_normal_map.frag");
		load_shader("skybox", "src/Shaders/skybox.vert", "src/Shaders/skybox.frag");

		// Load all the skyboxes
		load_skybox("WaterAndMountains", "data/CubeMaps/Skyboxes/WaterAndMountains");
		load_skybox("Night", "data/CubeMaps/Skyboxes/Night");
		load_skybox("ClearSky", "data/CubeMaps/Skyboxes/ClearSky");
		load_skybox("Sunny", "data/CubeMaps/Skyboxes/Sunny");
	}


	// Release all the resources allocated.
	void ResourceManager::clear_resources()
	{
		clear_models();
		clear_shaders();
		clear_skyboxes();
	}

	void ResourceManager::clear_models()
	{
		// Free the memory of the models
		for (auto it : m_models)
		{
			if (it.second != nullptr)
			{
				delete it.second;
				it.second = nullptr;
			}
		}

		m_models.clear();
	}

	void ResourceManager::clear_shaders()
	{
		// Free the memory of the shaders
		for (auto it : m_shaders)
		{
			if (it.second != nullptr)
			{
				delete it.second;
				it.second = nullptr;
			}
		}

		m_shaders.clear();
	}

	void ResourceManager::clear_skyboxes()
	{
		// Free the memory of the skyboxes
		for (auto it : m_skyboxes)
		{
			if (it.second != nullptr)
			{
				delete it.second;
				it.second = nullptr;
			}
		}

		m_skyboxes.clear();
	}


	// Get the model associated to the filePath parameter.
	// Loads it if it is not already loaded.
	Model* ResourceManager::get_model(const std::string& filePath)
	{
		auto foundIt = m_models.find(filePath);

		// If no model with filePath as key exists, load it
		if (foundIt == m_models.end())
		{
			Model* newModel = new Model;
			newModel->load_gltf_file(filePath);

			m_models[filePath] = newModel;
		}

		return m_models[filePath];
	}


	// Get the shader associated to the name provided.
	// Returns nullptr if the shader hasn't been loaded already.
	Shader* ResourceManager::get_shader(const std::string& shaderIdName)
	{
		auto foundIt = m_shaders.find(shaderIdName);

		// If not found it, return null
		if (foundIt == m_shaders.end())
		{
			std::cout << "WARNING: The requested shader was not found in the resource manager\n";
			return nullptr;
		}

		return foundIt->second;
	}

	// Loads, compiles and links the vertex and fragment shaders provided, and stores the resulting program in the
	// resource manager, with shaderIdName as key. Any calls to get_shader should have this key as parameter to
	// retreive again the shader resource.
	void ResourceManager::load_shader(const std::string& shaderIdName, const std::string& vertexPath, const std::string& fragmentPath)
	{
		auto foundIt = m_shaders.find(shaderIdName);

		if (foundIt != m_shaders.end())
		{
			std::cout << "ERROR: Shader with key \"" << shaderIdName << "\" already exists\n";
			return;
		}

		Shader* newShader = new Shader;
		newShader->create_shader_program(vertexPath, fragmentPath);
		m_shaders[shaderIdName] = newShader;
	}

	// Get the skybox associated to the name provided.
	// Loads the skybox if it hasn't been already loaded.
	// Returns nullptr if the shader hasn't been loaded already.
	Skybox* ResourceManager::get_skybox(const std::string& skyboxIdName)
	{
		auto foundIt = m_skyboxes.find(skyboxIdName);

		// If not found it, return null
		if (foundIt == m_skyboxes.end())
		{
			std::cout << "WARNING: The requested skybox was not found in the resource manager\n";
			return nullptr;
		}

		return foundIt->second;
	}

	// Loads the cubemap textures in skyboxPath used for a skybox, and stores the resulting skybox object in the
	// resource manager, with skyboxIdName as key. Any calls to get_skybox should have this key as parameter to
	// retreive again the skybox resource.
	void ResourceManager::load_skybox(const std::string& skyboxIdName, const std::string& skyboxDirPath)
	{
		auto foundIt = m_skyboxes.find(skyboxIdName);

		if (foundIt != m_skyboxes.end())
		{
			std::cout << "ERROR: Skybox with key \"" << skyboxIdName << "\" already exists\n";
			return;
		}

		Skybox* newSkybox = new Skybox;
		newSkybox->load_skybox(skyboxDirPath);
		m_skyboxes[skyboxIdName] = newSkybox;
	}

	// Get the cube geometry
	Cube& ResourceManager::get_cube()
	{
		return m_cube;
	}
}