/**
* @file IKChain.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Contains the inverse kinematics chain of nodes.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "IKChain.h"
#include "Composition/SceneNode.h"


namespace cs460
{
	IKChain::IKChain(SceneNode* chainRoot, SceneNode* endEffector)
		:	m_chainRoot(chainRoot),
			m_endEffector(endEffector)
	{
	}


	void IKChain::set_chain_root(SceneNode* chainRoot)
	{
		m_chainRoot = chainRoot;
	}

	void IKChain::set_end_effector(SceneNode* endEffector)
	{
		m_endEffector = endEffector;
	}

	SceneNode* IKChain::get_chain_root()
	{
		return m_chainRoot;
	}

	SceneNode* IKChain::get_end_effector()
	{
		return m_endEffector;
	}
}