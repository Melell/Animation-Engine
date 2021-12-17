/**
* @file Cloth.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Component used for a cloth simulation.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Cloth.h"
#include "ClothConstraints.h"
#include "Animation/ParticleSimulations/ClothMgr.h"
#include "Graphics/Systems/DebugRenderer.h"
#include "Math/Geometry/Geometry.h"
#include "Resources/ResourceManager.h"


namespace cs460
{
	Cloth::Cloth()
	{
		ClothMgr::get_instance().add_cloth(this);
	}

	Cloth::~Cloth()
	{
		// Clean the memory for the added constraints
		for (unsigned i = 0; i < m_system.m_constraints.size(); ++i)
		{
			delete m_system.m_constraints[i];
			m_system.m_constraints[i] = nullptr;
		}

		ClothMgr::get_instance().remove_cloth(this);
	}


	// Initialize the particles as well as the constraints
	void Cloth::initialize()
	{
		// Initialize the positions of the particles (as well as the dimensions)
		initialize_particle_grid();

		// Initialize the uv coordinates of each particle
		initialize_uvs();

		// Add the constraints
		initialize_stretch_constraints();
	}

	// Update the particle system (verlet integration, constraints satisfaction etc)
	void Cloth::update()
	{
		m_system.update();
	}

	// Debug draws this cloth
	void Cloth::debug_draw()
	{
		// Debug draw all the horizontal links
		for (unsigned r = 0; r < m_height; ++r)
		{
			for (unsigned c = 0; c < m_width - 1; ++c)
			{
				unsigned currIdx = idx(r, c);
				unsigned nextIdx = idx(r, c + 1);
		
				// Get the current and the next particles
				VerletParticle& part0 = m_system.m_particles[currIdx];
				VerletParticle& part1 = m_system.m_particles[nextIdx];
		
				// Create a segment between them and draw it
				Segment seg;
				seg.m_start = part0.m_pos;
				seg.m_end = part1.m_pos;
				DebugRenderer::draw_segment(seg, { 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}
		
		// Debug draw all the vertical links
		for (unsigned r = 0; r < m_height - 1; ++r)
		{
			for (unsigned c = 0; c < m_width ; ++c)
			{
				unsigned currIdx = idx(r, c);
				unsigned nextIdx = idx(r + 1, c);
		
				// Get the current and the next particles
				VerletParticle& part0 = m_system.m_particles[currIdx];
				VerletParticle& part1 = m_system.m_particles[nextIdx];
		
				// Create a segment between them and draw it
				Segment seg;
				seg.m_start = part0.m_pos;
				seg.m_end = part1.m_pos;
				DebugRenderer::draw_segment(seg, { 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}
	}

	// Draw with a texture (currently hardcoded)
	void Cloth::draw_textured()
	{
		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Plane& plane = resourceMgr.get_plane();

		plane.bind_diffuse();
		plane.bind_normal();
		plane.bind_geometry();
	}


	// Getters for number of particles in width and height
	unsigned Cloth::get_width() const
	{
		return m_width;
	}

	unsigned Cloth::get_height() const
	{
		return m_height;
	}

	VerletParticleSystem& Cloth::get_system()
	{
		return m_system;
	}


	void Cloth::initialize_particle_grid()
	{
		// Initialize the dimensions of the grid
		m_width = 15;
		m_height = 24;
		m_system.m_particlesInUse = m_width * m_height;

		float restLength = 0.2f;

		// Initialize the positions of the particles
		for (unsigned r = 0; r < m_height; ++r)
		{
			for (unsigned c = 0; c < m_width; ++c)
			{
				VerletParticle& part = m_system.m_particles[idx(r, c)];
				part.m_pos = part.m_oldPos = glm::vec3(c * restLength, 0.0, r * restLength);
			}
		}

		// Set the particles of the top row as not movable
		for (unsigned c = 0; c < m_width; ++c)
			m_system.m_particles[c].m_canMove = false; //.m_invMass = 0.0f;
	}


	void Cloth::initialize_uvs()
	{
		float xUVOffset = 1.0f / (float)m_width;
		float yUVOffset = 1.0f / (float)m_height;

		for (unsigned r = 0; r < m_height; ++r)
		{
			for (unsigned c = 0; c < m_width; ++c)
			{
				VerletParticle& part = m_system.m_particles[idx(r, c)];
				part.m_uv = glm::vec2((float)c * xUVOffset, (float)r * yUVOffset);
			}
		}
	}


	// Add all the strech constraints (horizontal and vertical links)
	void Cloth::initialize_stretch_constraints()
	{
		// Add all the horizontal links
		for (unsigned r = 0; r < m_height; ++r)
		{
			for (unsigned c = 0; c < m_width - 1; ++c)
			{
				unsigned currIdx = idx(r, c);
				unsigned nextIdx = idx(r, c + 1);

				// Get the current and the next particles
				VerletParticle& part0 = m_system.m_particles[currIdx];
				VerletParticle& part1 = m_system.m_particles[nextIdx];

				// Create the constraint, set its parameters, and add it
				RestLengthConstraint* constraint = new RestLengthConstraint;
				constraint->m_part0 = currIdx;
				constraint->m_part1 = nextIdx;
				constraint->m_restLength = glm::length(part1.m_pos - part0.m_pos);
				m_system.m_constraints.push_back(constraint);
			}
		}

		// Add all the vertical links
		for (unsigned r = 0; r < m_height - 1; ++r)
		{
			for (unsigned c = 0; c < m_width; ++c)
			{
				unsigned currIdx = idx(r, c);
				unsigned nextIdx = idx(r + 1, c);

				// Get the current and the next particles
				VerletParticle& part0 = m_system.m_particles[currIdx];
				VerletParticle& part1 = m_system.m_particles[nextIdx];

				// Create the constraint, set its parameters, and add it
				RestLengthConstraint* constraint = new RestLengthConstraint;
				constraint->m_part0 = currIdx;
				constraint->m_part1 = nextIdx;
				constraint->m_restLength = glm::length(part1.m_pos - part0.m_pos);
				m_system.m_constraints.push_back(constraint);
			}
		}
	}


	void Cloth::on_gui()
	{
	}


	unsigned Cloth::idx(unsigned row, unsigned column) const
	{
		return row * m_width + column;
	}
}