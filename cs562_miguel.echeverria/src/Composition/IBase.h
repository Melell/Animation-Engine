/**
* @file IBase.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Base interface for all classes that are meant to be created through
*		 factory, for polymorphism and convenient casting.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	// Base interface for all classes that are meant to be created through
	// factory, for polymorphism and convenient casting.
	struct IBase
	{
	public:

		virtual ~IBase();

	protected:
		IBase();	// Protected so that an IBase can't be created.
	};
}