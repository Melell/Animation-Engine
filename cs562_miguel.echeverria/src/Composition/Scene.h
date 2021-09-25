/**
* @file Scene.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the declaration of the scene singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Scene
	{
	public:
	
		~Scene();
		static Scene& get_instance();
	
	private:
	
		
		// For singleton pattern
		Scene();
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;
	};
}