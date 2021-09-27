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


namespace cs460
{
	class IComponent;


	class SceneNode
	{
	public:
		
		SceneNode(const std::string& name = "Unnamed");
		~SceneNode();

		std::string m_name;			// "Gameobject" name
		TransformData m_localTr;	// Local transform (with respect to parent)
		TransformData m_worldTr;	// World transform (with respect world origin)

		SceneNode* create_child(const std::string& name);		// Create a new node and add it as this node's child

		// Component management functions ----------------
		template<typename T>
		T* add_component();

		template<typename T>
		T* get_component();

		template<typename T>
		void delete_component();

		template<typename T>
		void delete_component(T* compToDelete);
		// -----------------------------------------------

		void on_gui();	// Show the components gui

		// Getters for the parent and children nodes, as well as the components
		SceneNode* get_parent() const;
		const std::vector<SceneNode*>& get_children() const;
		const std::vector<IComponent*>& get_components() const;

	private:
		SceneNode* m_parent;
		std::vector<SceneNode*> m_children;
		std::vector<IComponent*> m_components;

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
	T* SceneNode::get_component()
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