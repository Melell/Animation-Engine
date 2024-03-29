/**
* @file SceneNode.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Declaration of the class that represents a node in the scene
*		 graph (like a gameobject), already containing transform information.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "TransformData.h"
#include "Scene.h"

namespace tinygltf
{
	class Model;
	class Node;
}


namespace cs460
{
	class IComponent;
	struct Model;
	class ModelInstance;
	struct GLTFNode;


	class SceneNode
	{
	public:
		
		friend class Scene;								// Scene can access private members of SceneNode

		SceneNode(const std::string& name = "Unnamed");
		~SceneNode();

		TransformData m_localTr;	// Local transform (with respect to parent)
		TransformData m_worldTr;	// World transform (with respect world origin)

		void delete_all_children();			// Free all the children of this node
		void delete_all_components();		// Free all the components of this node

		// Delete all the children with a component of type T
		template<typename T>
		void delete_all_children_with_comp();

		// Create a new node and add it as this node's child
		SceneNode* create_child(const std::string& name);

		// Create a new node with a model instance component and add it as this node's child.
		// The model instance component will load/get the model at the given path.
		SceneNode* create_child_with_model(const std::string& name, const fs::path& modelPath);

		// Generates all the data for this scenenode from a Model resource (except the necessary components)
		void from_node_resource(Model* sourceModel, int nodeIdx, ModelInstance* rootModelInst);

		// Generate the components necessary for this node (from the gltf node info), and its children
		void generate_components(int nodeIdx);

		// -------------------------- Component management functions --------------------------
		template<typename T>
		T* add_component();

		template<typename T>
		T* get_component() const;

		template<typename T>
		void delete_component();

		template<typename T>
		void delete_component(T* compToDelete);
		// ------------------------------------------------------------------------------------

		void on_gui();	// Show the components gui

		// Getters for the parent and children nodes, as well as the components, and the Model resource this node belongs to
		SceneNode* get_parent() const;
		const std::vector<SceneNode*>& get_children() const;
		const std::vector<IComponent*>& get_components() const;
		SceneNode* get_model_root_node() const;
		Model* get_model() const;

		// Setters for the model referenced by this node, as well as the root node of the model's hierarchy
		void set_model_source(Model* newModel);
		void set_model_root_node(SceneNode* modelRootNode);

		// Getter for the unique id of this scene node
		unsigned get_unique_id() const;

		// Setter and getter for the name
		void change_name(const std::string& newName);
		std::string get_name() const;


		// Needs to be set to false each frame that we don't want to update this node's world transform
		bool m_updateWorldTr = true;

	private:
		std::string m_name;						// "Gameobject" name
		SceneNode* m_parent;
		std::vector<SceneNode*> m_children;
		std::vector<IComponent*> m_components;
		SceneNode* m_modelRootNode;				// The root node of the model's hierarchy (null if it doesn't belong to a model)
		Model* m_sourceModel;					// The gltf model resource this node belongs to (null if doesn't belong to any model)
		unsigned m_UID;

		static unsigned s_UIDGenerator;


		void show_transforms_gui();

		// Helper functions for creating and adding the meshrenderable and skin components to this SceneNode if necessary.
		void generate_mesh_comp(const GLTFNode& node);
		void generate_skin_comps(const GLTFNode& node, std::unordered_map<int, SceneNode*>& modelInstNodes);
	};



	// Delete all the children with a component of type T
	template<typename T>
	void SceneNode::delete_all_children_with_comp()
	{
		// For each child
		for (auto it = m_children.begin(); it != m_children.end();)
		{
			SceneNode* child = *it;

			// If it has the component, delete its subtree, and erase it as child of its parent
			if (child->get_component<T>())
			{
				Scene& scene = Scene::get_instance();
				it = m_children.erase(it);
				scene.delete_tree(child);//, false);
				continue;
			}

			// Otherwise, go into this child's children
			child->delete_all_children_with_comp<T>();

			++it;
		}
	}

	
	template<typename T>
	T* SceneNode::add_component()
	{
		T* comp = get_component<T>();

		// Can't add the same component twice
		if (comp != nullptr)
			return comp;

		comp = new T;
		m_components.push_back(comp);
		comp->set_owner(this);
		return comp;
	}

	template<typename T>
	T* SceneNode::get_component() const
	{
		for (int i = 0; i < m_components.size(); ++i)
			if (T* comp = dynamic_cast<T*>(m_components[i]))
				return comp;

		// Not found, return nullptr
		return nullptr;
	}

	template<typename T>
	void SceneNode::delete_component()
	{
		delete_component(get_component<T>());
	}

	template<typename T>
	void SceneNode::delete_component(T* compToDelete)
	{
		if (compToDelete == nullptr)
			return;

		auto foundIt = std::find(m_components.begin(), m_components.end(), compToDelete);

		if (foundIt != m_components.end())
		{
			m_components.erase(foundIt);
			delete compToDelete;
		}
	}
}