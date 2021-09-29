/**
* @file ResourceManager.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Singleton that acts as a pool of resources (models, textures, etc).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Graphics/Model.h"

namespace cs460
{
	class ResourceManager
	{
	public:

		~ResourceManager();
		static ResourceManager& get_instance();

		// Release all the resources allocated.
		void clear_resources();

		// Get the model associated to the filePath parameter.
		// Loads it if it is not already loaded.
		Model* get_model(const std::string& filePath);

	private:

		std::unordered_map<std::string, Model*> m_models;

		ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
	};
}