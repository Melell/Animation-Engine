/**
* @file VerletParticleSystem.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Verlet particle system used for the cloth simulation.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "VerletParticleSystem.h"
#include "IConstraint.h"



namespace cs460
{
	void VerletParticle::add_force(const glm::vec3& force)
	{
		m_acceleration += force;// * m_invMass;
	}


	void VerletParticleSystem::update()
	{
		add_gravity();
		//add_wind();
		verlet_integration();
		solve_constraints();
		clear_forces();
	}


	// Add the gravity force to each particle in use
	void VerletParticleSystem::add_gravity()
	{
		for (unsigned i = 0; i < m_particlesInUse; ++i)
			m_particles[i].add_force(m_gravity);
	}


	// Add the wind force to each particle in use
	void VerletParticleSystem::add_wind()
	{
		for (unsigned i = 0; i < m_particlesInUse; ++i)
			m_particles[i].add_force(m_wind);
	}


	// Verlet integration with a fixed timestep
	void VerletParticleSystem::verlet_integration()
	{
		for (unsigned i = 0; i < m_particlesInUse; ++i)
		{
			VerletParticle& part = m_particles[i];
			if (!part.m_canMove)
				continue;

			glm::vec3 temp = part.m_pos;

			float drag = 0.997f;

			part.m_pos += (part.m_pos - part.m_oldPos) * drag + part.m_acceleration * m_fixedDt * m_fixedDt;
			part.m_oldPos = temp;
		}
	}


	// Satisfy the given constraints
	void VerletParticleSystem::solve_constraints()
	{
		// Solve each constraints m_constraintIterations times
		for (unsigned i = 0; i < m_constraintIterations; ++i)
			for (unsigned c = 0; c < m_constraints.size(); ++c)
				m_constraints[c]->solve(this);
	}


	// Clear the acceleration of each particle
	void VerletParticleSystem::clear_forces()
	{
		for (unsigned i = 0; i < m_particlesInUse; ++i)
			m_particles[i].m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}