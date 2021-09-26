/**
* @file Scene.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the implementation of the scene singleton class.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Scene.h"
#include "SceneNode.h"


namespace cs460
{
	Scene::Scene()
		:	m_root(new SceneNode("ROOT"))
	{

	}
	
	Scene::~Scene()
	{
	}
	
	
	Scene& Scene::get_instance()
	{
		static Scene instance;
		return instance;
	}
	SceneNode* Scene::get_root() const
	{
		return m_root;
	}
}