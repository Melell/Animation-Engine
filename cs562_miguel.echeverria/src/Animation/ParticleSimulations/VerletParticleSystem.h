/**
* @file VerletParticleSystem.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Verlet particle system used for the cloth simulation.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/Particles/ClothConstraints.h"

namespace cs460
{
	const unsigned MAX_PARTICLES = 1024;

	struct IConstraint;


	struct VerletParticle
	{
		glm::vec3 m_pos{0.0f, 0.0f, 0.0f};
		glm::vec3 m_oldPos{0.0f, 0.0f, 0.0f};
		glm::vec3 m_acceleration{0.0f, 0.0f, 0.0f};
		glm::vec2 m_uv{ 0.0f, 0.0f };					// Optional: Used by cloth
		glm::vec3 m_normal{0.0f, 1.0f, 0.0f};			// Useful to store it here as it will be used by the wind
		bool m_canMove = true;


		void add_force(const glm::vec3& force);
	};


	struct VerletParticleSystem
	{
		VerletParticle m_particles[MAX_PARTICLES];
		glm::vec3 m_gravity{ 0.0f, -9.8f, 0.0f };
		glm::vec3 m_wind{ 0.0f, 0.0f, 1.0f };
		float m_drag = 0.997f;
		unsigned m_particlesInUse = 5 * 10;

		unsigned m_constraintIterations = 1;
		const float m_fixedDt = 1.0f / 60.0f;
		std::vector<IConstraint*> m_constraints;

		void update();

	private:

		void add_gravity();
		void add_wind();
		void verlet_integration();
		void solve_constraints();
		void clear_forces();
	};
}