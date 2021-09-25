/**
* @file Engine.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief The Engine singleton class is a container and manager for the different systems of the engine.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Engine
	{
	public:
	
		bool initialize();	// Initialization of the engine and all its systems
		void update();		// Update of the engine and all its necessary systems
		void close();		// Close the engine by releasing any resources in use

	private:
		
	};
}