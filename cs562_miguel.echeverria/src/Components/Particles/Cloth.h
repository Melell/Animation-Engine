/**
* @file Cloth.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Component used for a cloth simulation.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"
#include "Animation/ParticleSimulations/VerletParticleSystem.h"


namespace cs460
{
	class Cloth : public IComponent
	{
	public:

		Cloth();
		virtual ~Cloth();

		void initialize();		// Initialize the particles as well as the constraints
		void update();			// Update the particle system (verlet integration, constraints satisfaction etc)
		void debug_draw();		// Debug draws this cloth
		void draw_textured();	// Draw with a texture (currently hardcoded)

		// Getters for number of particles in width and height
		unsigned get_width() const;
		unsigned get_height() const;

		VerletParticleSystem& get_system();

	private:

		VerletParticleSystem m_system;
		unsigned m_width = 1;
		unsigned m_height = 1;

		void initialize_particle_grid();
		void initialize_uvs();
		void initialize_stretch_constraints();

		void on_gui() override;

		unsigned idx(unsigned row, unsigned column) const;
	};
}