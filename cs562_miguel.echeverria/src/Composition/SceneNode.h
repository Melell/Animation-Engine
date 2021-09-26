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
	class SceneNode
	{
	public:
		
		SceneNode(const std::string& name = "Unnamed");
		~SceneNode();

		std::string m_name;			// "Gameobject" name
		TransformData m_localTr;	// Local transform (with respect to parent)
		TransformData m_worldTr;	// World transform (with respect world origin)

		void create_child(const std::string& name);		// Create a new node and add it as this node's child

		// Getters for the parent and children nodes
		SceneNode* get_parent() const;
		const std::vector<SceneNode*>& get_children() const;

	private:
		SceneNode* m_parent;
		std::vector<SceneNode*> m_children;
	};
}