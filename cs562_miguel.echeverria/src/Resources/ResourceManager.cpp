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


	// Release all the resources allocated.
	void ResourceManager::clear_resources()
	{
		for (auto it : m_models)
		{
			if (it.second != nullptr)
			{
				delete it.second;
				it.second = nullptr;
			}
		}
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
			newModel->load_gltf(filePath);

			m_models[filePath] = newModel;
		}

		return m_models[filePath];
	}
}