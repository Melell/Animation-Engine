/**
* @file ClothConstraints.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Contains the specific constraints that need to be solved for a cloth simulation.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ClothConstraints.h"
#include "Animation/ParticleSimulations/VerletParticleSystem.h"


namespace cs460
{
	void RestLengthConstraint::solve(VerletParticleSystem* system)
	{
		if (system == nullptr)
			return;

		// Get each of the particles affected by the constraint
		VerletParticle& part0 = system->m_particles[m_part0];
		VerletParticle& part1 = system->m_particles[m_part1];

		// Satisfy the constraint (code from Advanced Character Physics paper)
		const glm::vec3& delta = part1.m_pos - part0.m_pos;
		float deltalength = glm::length(delta);
		float diff = (deltalength - m_restLength) / deltalength;

		if (part0.m_canMove && part1.m_canMove)
		{
			part0.m_pos += delta * 0.5f * diff;
			part1.m_pos -= delta * 0.5f * diff;
		}
		else if (part0.m_canMove)
		{
			part0.m_pos += delta * diff;
		}
		else if (part1.m_canMove)
		{
			part1.m_pos -= delta * diff;
		}
	}
}