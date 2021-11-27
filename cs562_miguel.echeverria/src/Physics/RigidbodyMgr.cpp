/**
* @file RigidbodyMgr.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Contains all the rigidbody components, and updates them.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "RigidbodyMgr.h"
#include "Components/Physics/Rigidbody.h"


namespace cs460
{
	RigidbodyMgr& RigidbodyMgr::get_instance()
	{
		static RigidbodyMgr instance;
		return instance;
	}

	RigidbodyMgr::RigidbodyMgr()
	{
	}

	RigidbodyMgr::~RigidbodyMgr()
	{
	}


	void RigidbodyMgr::add_rigidbody(Rigidbody* rigidbody)
	{
		m_rigidbodies.push_back(rigidbody);
	}

	void RigidbodyMgr::remove_rigidbody(Rigidbody* rigidbody)
	{
		auto foundIt = std::find(m_rigidbodies.begin(), m_rigidbodies.end(), rigidbody);

		// Nothing to remove if not in the vector
		if (foundIt == m_rigidbodies.end())
			return;

		m_rigidbodies.erase(foundIt);
	}
}