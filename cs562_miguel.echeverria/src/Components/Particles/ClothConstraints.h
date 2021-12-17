/**
* @file ClothConstraints.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Contains the specific constraints that need to be solved for a cloth simulation.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Animation/ParticleSimulations/IConstraint.h"


namespace cs460
{
	struct VerletParticleSystem;


	struct RestLengthConstraint : public IConstraint
	{
		// The particles affected and the length they need to mantain
		unsigned m_part0 = 0;
		unsigned m_part1 = 1;
		float m_restLength = 1.0f;

		void solve(VerletParticleSystem* system) override;
	};
}