/**
* @file IConstraint.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Base class for Verlet Particle System constraints.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct VerletParticleSystem;


	struct IConstraint
	{
		virtual void solve(VerletParticleSystem* system) = 0;
	};
}