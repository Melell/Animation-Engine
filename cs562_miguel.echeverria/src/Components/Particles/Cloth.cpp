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
#include "Graphics/Rendering/Shader.h"
#include "Composition/Scene.h"
#include "Cameras/ICamera.h"
#include "Utilities/ImageProcessing.h"
#include <GL/glew.h>


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

		delete_gl_buffers();

		ClothMgr::get_instance().remove_cloth(this);
	}


	// Initialize the particles as well as the constraints
	void Cloth::initialize(SceneNode* sphere)
	{
		// Initialize the positions of the particles (as well as the dimensions)
		initialize_particle_grid();

		// Initialize the uv coordinates of each particle
		initialize_uvs();

		// Initialize all the buffers for rendering
		initialize_render_data();

		// Initialize the diffuse texture
		initialize_texture_data();

		// Add the constraints
		initialize_all_constraints(sphere);
	}

	// Update the particle system (verlet integration, constraints satisfaction etc)
	void Cloth::update()
	{
		m_system.update();
		update_normals();
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
		setup_uniforms();

		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;

		// Update the render data (positions and normals)
		for (unsigned r = 0; r < m_height - 1; ++r)
		{
			glBindVertexArray(m_vaos[r]);

			for (unsigned c = 0; c < m_width; ++c)
			{
				unsigned currIdx = idx(r, c);
				unsigned nextIdx = idx(r + 1, c);

				VerletParticle& currPart = m_system.m_particles[currIdx];
				VerletParticle& nextPart = m_system.m_particles[nextIdx];

				// Gather the position data from the particles
				pos.push_back(currPart.m_pos);
				pos.push_back(nextPart.m_pos);

				// Gather the averaged normals from the particles
				normals.push_back(currPart.m_normal);//get_averaged_normal(r, c));
				normals.push_back(nextPart.m_normal);//get_averaged_normal(r + 1, c));

				// Gather the averaged tangents from the particles
				tangents.push_back(get_averaged_tangent(r, c));
				tangents.push_back(get_averaged_tangent(r + 1, c));
			}

			// Update the position data
			glBindBuffer(GL_ARRAY_BUFFER, m_posVbos[r]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, pos.size() * sizeof(glm::vec3), pos.data());

			// Update the normals data
			glBindBuffer(GL_ARRAY_BUFFER, m_normalVbos[r]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(glm::vec3), normals.data());

			// Update the tangents data
			glBindBuffer(GL_ARRAY_BUFFER, m_tangentVbos[r]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, tangents.size() * sizeof(glm::vec3), tangents.data());

			// Draw the current triangle strip
			glDrawArrays(GL_TRIANGLE_STRIP, 0, pos.size());

			pos.clear();
			normals.clear();
			tangents.clear();

			glBindVertexArray(0);
		}
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
		m_width = 16;
		m_height = 18;
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
		//for (unsigned c = 0; c < m_width; ++c)
		//	m_system.m_particles[c].m_canMove = false; //.m_invMass = 0.0f;
		m_system.m_particles[0].m_canMove = false;
		m_system.m_particles[m_width - 1].m_canMove = false;
	}


	void Cloth::initialize_uvs()
	{
		float xTiling = 1.0f;
		float yTiling = 1.0f;

		float xUVOffset = 1.0f / ((float)m_width * xTiling);
		float yUVOffset = 1.0f / ((float)m_height * yTiling);
		
		for (unsigned r = 0; r < m_height; ++r)
		{
			for (unsigned c = 0; c < m_width; ++c)
			{
				VerletParticle& part = m_system.m_particles[idx(r, c)];
				part.m_uv = glm::vec2((float)c * xUVOffset, (float)r * yUVOffset);
			}
		}
	}


	void Cloth::initialize_render_data()
	{
		m_vaos.resize(m_height - 1);
		m_posVbos.resize(m_height - 1);
		m_normalVbos.resize(m_height - 1);
		m_texCoordsVbos.resize(m_height - 1);
		m_tangentVbos.resize(m_height - 1);

		// Generate the buffers for each triangle strip
		glGenVertexArrays(m_height - 1, m_vaos.data());
		glGenBuffers(m_height - 1, m_posVbos.data());
		glGenBuffers(m_height - 1, m_normalVbos.data());
		glGenBuffers(m_height - 1, m_texCoordsVbos.data());
		glGenBuffers(m_height - 1, m_tangentVbos.data());

		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> tangents;
		

		// Upload the position, normals and uvs' data
		for (unsigned r = 0; r < m_height - 1; ++r)
		{
			glBindVertexArray(m_vaos[r]);

			for (unsigned c = 0; c < m_width; ++c)
			{
				unsigned currIdx = idx(r, c);
				unsigned nextIdx = idx(r + 1, c);

				VerletParticle& currPart = m_system.m_particles[currIdx];
				VerletParticle& nextPart = m_system.m_particles[nextIdx];

				// Gather the position data from the particles
				pos.push_back(currPart.m_pos);
				pos.push_back(nextPart.m_pos);

				// Gather the averaged normals from the particles
				currPart.m_normal = get_averaged_normal(r, c);
				nextPart.m_normal = get_averaged_normal(r + 1, c);
				normals.push_back(currPart.m_normal);
				normals.push_back(nextPart.m_normal);

				// Gather the uv data from the particles
				uvs.push_back(currPart.m_uv);
				uvs.push_back(nextPart.m_uv);

				// Gather the averaged tangents from the particles
				tangents.push_back(get_averaged_tangent(r, c));
				tangents.push_back(get_averaged_tangent(r + 1, c));
			}

			// Upload the position data
			glBindBuffer(GL_ARRAY_BUFFER, m_posVbos[r]);
			glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), pos.data(), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			// Upload the normals data
			glBindBuffer(GL_ARRAY_BUFFER, m_normalVbos[r]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			// Upload the texture coordinates' data
			glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsVbos[r]);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			// Upload the tangents data
			glBindBuffer(GL_ARRAY_BUFFER, m_tangentVbos[r]);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			pos.clear();
			normals.clear();
			uvs.clear();
			tangents.clear();

			glBindVertexArray(0);
		}
	}


	void Cloth::initialize_texture_data()
	{
		// Set the texture units for diffuse and normal
		m_diffuse.set_texture_unit(0);
		m_normalMap.set_texture_unit(1);

		TextureInformation diffuseInfo;
		TextureInformation normalInfo;

		// Load both textures
		int channelsInFile = 1;
		unsigned char* diffuseData = load_image("data/Textures/Cloth/RippedCloth/RippedCloth_Albedo.png", diffuseInfo.m_width, diffuseInfo.m_height, channelsInFile, 4);
		unsigned char* normalData = load_image("data/Textures/Cloth/RippedCloth/RippedCloth_Normal.png", normalInfo.m_width, normalInfo.m_height, channelsInFile, 4);


		// Bind the diffuse texture
		m_diffuse.bind();

		// Upload the diffuse texture data
		m_diffuse.upload_data(diffuseData, diffuseInfo);


		// Bind the normal texture
		m_normalMap.bind();

		// Upload the normal texture data
		m_normalMap.upload_data(normalData, normalInfo);
	}


	void Cloth::initialize_all_constraints(SceneNode* sphere)
	{
		initialize_sphere_collision_constraints(sphere);
		initialize_stretch_constraints();
	}

	void Cloth::initialize_sphere_collision_constraints(SceneNode* sphere)
	{
		if (sphere == nullptr)
			return;

		// For each particle in use
		for (unsigned i = 0; i < m_system.m_particlesInUse; ++i)
		{
			// Create the constraint, set its parameters, and add it
			SphereCollisionConstraint* constraint = new SphereCollisionConstraint;
			constraint->m_part = i;
			constraint->m_sphere = sphere;
			m_system.m_constraints.push_back(constraint);
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


	// Update the average normals of each particle
	void Cloth::update_normals()
	{
		for (int r = 0; r < m_height; ++r)
		{
			for (int c = 0; c < m_width; ++c)
			{
				m_system.m_particles[idx(r, c)].m_normal = get_averaged_normal(r, c);
			}
		}
	}


	glm::vec3 Cloth::get_averaged_normal(int row, int col)
	{
		unsigned normalCount = 0;
		glm::vec3 average(0.0f, 0.0f, 0.0f);

		// First quadrant
		if (col - 1 >= 0 && row - 1 >= 0)
		{
			VerletParticle& left = m_system.m_particles[idx(row, col - 1)];
			VerletParticle& top = m_system.m_particles[idx(row - 1, col)];
			VerletParticle& curr = m_system.m_particles[idx(row, col)];

			average += glm::cross(top.m_pos - curr.m_pos, left.m_pos - curr.m_pos);
			++normalCount;
		}

		// Second quadrant
		if (col + 1 < (int)m_width && row - 1 >= 0)
		{
			VerletParticle& right = m_system.m_particles[idx(row, col + 1)];
			VerletParticle& top = m_system.m_particles[idx(row - 1, col)];
			VerletParticle& curr = m_system.m_particles[idx(row, col)];

			average += glm::cross(right.m_pos - curr.m_pos, top.m_pos - curr.m_pos);
			++normalCount;
		}

		// Third quadrant
		if (col - 1 >= 0 && row + 1 < (int)m_height)
		{
			VerletParticle& left = m_system.m_particles[idx(row, col - 1)];
			VerletParticle& bottom = m_system.m_particles[idx(row + 1, col)];
			VerletParticle& curr = m_system.m_particles[idx(row, col)];

			average += glm::cross(left.m_pos - curr.m_pos, bottom.m_pos - curr.m_pos);
			++normalCount;
		}

		// Fourth quadrant
		if (col + 1 < (int)m_width && row + 1 < (int)m_height)
		{
			VerletParticle& right = m_system.m_particles[idx(row, col + 1)];
			VerletParticle& bottom = m_system.m_particles[idx(row + 1, col)];
			VerletParticle& curr = m_system.m_particles[idx(row, col)];

			average += glm::cross(bottom.m_pos - curr.m_pos, right.m_pos - curr.m_pos);
			++normalCount;
		}

		return glm::normalize(average / (float)normalCount);
	}


	glm::vec3 Cloth::get_averaged_tangent(int row, int col)
	{
		unsigned tangentCount = 0;
		glm::vec3 average(0.0f, 0.0f, 0.0f);
	
		// Left side tangent
		if (col - 1 >= 0)
		{
			VerletParticle& left = m_system.m_particles[idx(row, col - 1)];
			VerletParticle& curr = m_system.m_particles[idx(row, col)];
	
			glm::vec3 tangent = curr.m_pos - left.m_pos;
			average += tangent;
			++tangentCount;
		}

		// Right side tangent
		if (col + 1 < m_width)
		{
			VerletParticle& right = m_system.m_particles[idx(row, col + 1)];
			VerletParticle& curr = m_system.m_particles[idx(row, col)];

			glm::vec3 tangent = right.m_pos - curr.m_pos;
			average += tangent;
			++tangentCount;
		}
	
		return glm::normalize(average / (float)tangentCount);
	}


	void Cloth::setup_uniforms()
	{
		ResourceManager& resourceMgr = ResourceManager::get_instance();

		// Shader and uniform setup
		Shader* shader = resourceMgr.get_shader("phong_normal_map");
		shader->use();
		shader->set_uniform("mat.m_diffuse", m_diffuse.get_texture_unit());
		shader->set_uniform("mat.m_normalMap", m_normalMap.get_texture_unit());
		shader->set_uniform("mat.m_shininess", 8.0f);
		shader->set_uniform("normalColorScale", 1.0f);
		shader->set_uniform("useSkinning", false);
		
		Scene& scene = Scene::get_instance();
		ICamera* cam = scene.get_active_camera();
		shader->set_uniform("camPosWorldSpace", cam->get_position());
		shader->set_uniform("worldToView", cam->get_view_mtx());
		shader->set_uniform("perspectiveProj", cam->get_projection_mtx());
		shader->set_uniform("modelToWorld", glm::mat4(1.0f));
		shader->set_uniform("normalViewMtx", glm::transpose(glm::inverse(glm::mat3(cam->get_view_mtx() * glm::mat4(1.0f)))));
		
		// Set the light properties
		shader->set_uniform("light.m_direction", scene.m_lightProperties.m_direction);
		shader->set_uniform("light.m_ambient", scene.m_lightProperties.m_ambient);
		shader->set_uniform("light.m_diffuse", scene.m_lightProperties.m_diffuse);
		shader->set_uniform("light.m_specular", scene.m_lightProperties.m_specular);
	}


	void Cloth::delete_gl_buffers()
	{
		glDeleteVertexArrays(m_vaos.size(), m_vaos.data());
		glDeleteBuffers(m_posVbos.size(), m_posVbos.data());
		glDeleteBuffers(m_normalVbos.size(), m_normalVbos.data());
		glDeleteBuffers(m_texCoordsVbos.size(), m_texCoordsVbos.data());
	}


	void Cloth::on_gui()
	{
		// Wind parameters
		if (ImGui::InputFloat3("Wind Direction", glm::value_ptr(m_windDir), "%.2f"))
		{
			if (glm::epsilonNotEqual(glm::length(m_windDir), 0.0f, FLT_EPSILON))
				m_windDir = glm::normalize(m_windDir);
		}
		ImGui::SliderFloat("Wind Scale", &m_windScale, 0.0f, 20.0f, "%.2f");
		m_system.m_wind = m_windDir * m_windScale;
	}


	unsigned Cloth::idx(unsigned row, unsigned column) const
	{
		return row * m_width + column;
	}
}