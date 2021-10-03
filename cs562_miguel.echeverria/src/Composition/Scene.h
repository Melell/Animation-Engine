/**
* @file Scene.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the declaration of the scene singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Cameras/EditorCamera.h"


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

		void clear();							// Delete all the nodes in the scene graph except the root
		void delete_tree(SceneNode* node);		// Recursive function to free the memory of all the nodes in the given tree

		SceneNode* get_root() const;
	
		// The whole camera api will change
		EditorCamera& get_camera();

	private:
	
		SceneNode* m_root;
		EditorCamera m_camera;
		
		void update_node(SceneNode* node);		// Recursive function to update the transform of the provided node

		// For singleton pattern
		Scene();
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;
	};
}