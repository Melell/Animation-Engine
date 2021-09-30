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
	class SceneNode;


	class Scene
	{
	public:
	
		~Scene();
		static Scene& get_instance();

		// System management functions
		bool initialize();
		void update();
		void close();

		// Delete all the nodes in the scene graph
		void clear();

		SceneNode* get_root() const;
	
	private:
	
		SceneNode* m_root;
		
		void delete_tree(SceneNode* node);		// Recursive function to free the memory of all the nodes

		// For singleton pattern
		Scene();
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;
	};
}