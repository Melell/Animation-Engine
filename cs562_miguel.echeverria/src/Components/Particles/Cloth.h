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
#include "Graphics/Textures/Texture2D.h"


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
		std::vector<unsigned> m_vaos;
		std::vector<unsigned> m_posVbos;
		std::vector<unsigned> m_normalVbos;
		std::vector<unsigned> m_texCoordsVbos;
		std::vector<unsigned> m_tangentVbos;
		Texture2D m_diffuse;
		Texture2D m_normalMap;

		void initialize_particle_grid();
		void initialize_uvs();
		void initialize_render_data();
		void initialize_texture_data();
		void initialize_stretch_constraints();

		glm::vec3 get_averaged_normal(int row, int col);

		void setup_uniforms();

		void delete_gl_buffers();

		void on_gui() override;

		unsigned idx(unsigned row, unsigned column) const;
	};
}