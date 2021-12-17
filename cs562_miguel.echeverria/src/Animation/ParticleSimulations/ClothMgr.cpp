/**
* @file ClothMgr.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Updates all the cloth components currently active.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ClothMgr.h"
#include "Components/Particles/Cloth.h"


namespace cs460
{
	ClothMgr& ClothMgr::get_instance()
	{
		static ClothMgr instance;
		return instance;
	}

	ClothMgr::ClothMgr()
	{
	}

	ClothMgr::~ClothMgr()
	{
	}


	// Updates all the cloth components in order
	void ClothMgr::update()
	{
		for (Cloth* cloth : m_cloths)
			cloth->update();
	}

	// Uses debug drawing to render each cloth
	void ClothMgr::debug_draw()
	{
		for (Cloth* cloth : m_cloths)
			cloth->debug_draw();
	}

	// Draws all the cloths with a texture
	void ClothMgr::draw_textured()
	{
		for (Cloth* cloth : m_cloths)
			cloth->draw_textured();
	}


	void ClothMgr::add_cloth(Cloth* cloth)
	{
		m_cloths.push_back(cloth);
	}

	void ClothMgr::remove_cloth(Cloth* cloth)
	{
		auto foundIt = std::find(m_cloths.begin(), m_cloths.end(), cloth);

		// Nothing to remove if not in the vector
		if (foundIt == m_cloths.end())
			return;

		m_cloths.erase(foundIt);
	}
}