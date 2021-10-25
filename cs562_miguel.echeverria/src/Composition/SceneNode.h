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


	class SceneNode
	{
	public:
		
		friend class Scene;								// Scene can access private members of SceneNode

		SceneNode(const std::string& name = "Unnamed");
		~SceneNode();

		std::string m_name;			// "Gameobject" name
		TransformData m_localTr;	// Local transform (with respect to parent)
		TransformData m_worldTr;	// World transform (with respect world origin)

		void delete_all_children();			// Free all the children of this node
		void delete_all_components();		// Free all the components of this node

		// Create a new node and add it as this node's child
		SceneNode* create_child(const std::string& name);

		// Generates all the data for this scenenode from a Model resource
		void from_node_resource(Model* sourceModel, int nodeIdx, ModelInstance* rootModelInst);

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

	private:
		SceneNode* m_parent;
		std::vector<SceneNode*> m_children;
		std::vector<IComponent*> m_components;
		SceneNode* m_modelRootNode;				// The root node of the model's hierarchy (null if it doesn't belong to a model)
		Model* m_sourceModel;					// The gltf model resource this node belongs to (null if doesn't belong to any model)

		void show_transforms_gui();
	};



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