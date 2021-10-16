/**
* @file Skybox.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Cubemap that represents the sky.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Textures/CubeMap.h"


namespace cs460
{
	class Cube;


	class Skybox
	{
	public:

		Skybox();
		~Skybox();

		// Render the skybox. Supposed to be the last thing rendered in the scene.
		void render();

		// Start/stop using the skybox (rendering it)
		void set_active(bool newActive);
		bool get_active();

		void load_skybox(const std::string& newDirectoryPath);	// Loads the new skybox
		const std::string& get_directory() const;

	private:

		CubeMap m_cubeMap;
		std::string m_directory;
		bool m_isActive;
	};
}