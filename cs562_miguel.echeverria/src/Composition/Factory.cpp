/**
* @file Factory.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief System in charge of creating objects from names (names that match the class name).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Factory.h"


namespace cs460
{
	ICreator::ICreator()
	{
	}
	ICreator::~ICreator()
	{
	}


	Factory& Factory::get_instance()
	{
		static Factory instance;
		return instance;
	}

	Factory::Factory()
	{
	}

	Factory::~Factory()
	{
	}


	bool Factory::initialize()
	{


		return true;
	}

	void Factory::close()
	{

	}


	IBase* Factory::create(const std::string& className)
	{
		auto found = m_creators.find(className);

		if (found == m_creators.end())
			return nullptr;

		return found->second->create();
	}
}