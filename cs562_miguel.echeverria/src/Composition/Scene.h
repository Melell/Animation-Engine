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

	// Should this go on a separate file and inside a different class?
	struct LightProperties
	{
		glm::vec3 m_direction{-1.0f, -1.0f, -1.0f};
		glm::vec3 m_ambient{0.2f, 0.2f, 0.2f};
		glm::vec3 m_diffuse{0.5f, 0.5f, 0.5f};
		glm::vec3 m_specular{1.0f, 1.0f, 1.0f};
	};


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
		void delete_tree(SceneNode* node);//, bool clearParentChildren = true);		// Recursive function to free the memory of all the nodes in the given tree

		SceneNode* get_root() const;
	
		// The whole camera api will change
		EditorCamera& get_camera();

		std::vector<std::unordered_map<int, SceneNode*>>& get_all_model_nodes();
		std::unordered_map<int, SceneNode*>& get_model_inst_nodes(int instanceId);

		LightProperties m_lightProperties;

	private:
	
		SceneNode* m_root;
		std::vector<std::unordered_map<int, SceneNode*>> m_modelNodes;		// One "dictionary" per model instance in the scene
		EditorCamera m_camera;
		
		void update_node(SceneNode* node);		// Recursive function to update the transform of the provided node
		void delete_tree_internal(SceneNode* node);

		// For singleton pattern
		Scene();
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;
	};
}