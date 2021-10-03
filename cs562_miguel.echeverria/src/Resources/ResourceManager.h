/**
* @file ResourceManager.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Singleton that acts as a pool of resources (models, textures, etc).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Model;
	class Shader;


	class ResourceManager
	{
	public:

		~ResourceManager();
		static ResourceManager& get_instance();

		// Loads all the necessary resources for the demos. Meant to be called in Engine::Initialize.
		void load_resources();

		// Release all the resources allocated.
		void clear_resources();
		void clear_models();
		void clear_shaders();

		// Get the model associated to the filePath parameter.
		// Loads it if it is not already loaded.
		Model* get_model(const std::string& filePath);

		// Get the shader associated to the name provided.
		// Returns nullptr if the shader hasn't been loaded already.
		Shader* get_shader(const std::string& shaderIdName);

		// Loads, compiles and links the vertex and fragment shaders provided, and stores the resulting program in the
		// resource manager, with shaderIdName as key. Any calls to get_shader should have this key as parameter to
		// retreive again the shader resource.
		void load_shader(const std::string& shaderIdName, const std::string& vertexPath, const std::string& fragmentPath);

	private:

		std::unordered_map<std::string, Model*> m_models;
		std::unordered_map<std::string, Shader*> m_shaders;

		ResourceManager();
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
	};
}