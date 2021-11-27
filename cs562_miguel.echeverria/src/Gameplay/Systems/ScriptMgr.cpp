/**
* @file ScriptMgr.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Singleton class that stores all the existing script components
*		 and updates them.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ScriptMgr.h"
#include "Components/Gameplay/ScriptComponent.h"


namespace cs460
{
	ScriptMgr& ScriptMgr::get_instance()
	{
		static ScriptMgr instance;
		return instance;
	}

	ScriptMgr::ScriptMgr()
	{
	}

	ScriptMgr::~ScriptMgr()
	{
	}


	void ScriptMgr::initialize()
	{
		for (int i = 0; i < m_scripts.size(); ++i)
			m_scripts[i]->initialize();
	}

	void ScriptMgr::update()
	{
		for (int i = 0; i < m_scripts.size(); ++i)
			m_scripts[i]->update();
	}


	void ScriptMgr::add_script(ScriptComponent* scriptComp)
	{
		m_scripts.push_back(scriptComp);
	}

	void ScriptMgr::remove_script(ScriptComponent* scriptComp)
	{
		auto foundIt = std::find(m_scripts.begin(), m_scripts.end(), scriptComp);

		// Nothing to remove if not in the vector
		if (foundIt == m_scripts.end())
			return;

		m_scripts.erase(foundIt);
	}
}