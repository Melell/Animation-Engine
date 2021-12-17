/**
* @file Plane.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Basic plane that can be rendered. Uses totally harcoded textures
*		 for the cloth simulation demo.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Plane.h"
#include "Utilities/ImageProcessing.h"
#include <GL/glew.h>


namespace cs460
{
	Plane::Plane()
	{
		// Generate the vao, and vbo
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_posVbo);
		glGenBuffers(1, &m_normalsVbo);
		glGenBuffers(1, &m_texCoordsVbo);
		glGenBuffers(1, &m_tangentsVbo);

		// Bind the vao
		bind_geometry();

		float positions[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,

			-0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f
		};

		float normals[] = {
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f
		};

		float tangents[] = {
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
		};

		// Upload the position data
		glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// Upload the normals data
		glBindBuffer(GL_ARRAY_BUFFER, m_normalsVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// Upload the tangents data
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentsVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tangents), &tangents, GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


		// Unbind the vao
		unbind_geometry();


		// Upload the harcoded textures' data
		upload_texture_data();
	}

	Plane::~Plane()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_posVbo);
		glDeleteBuffers(1, &m_normalsVbo);
		glDeleteBuffers(1, &m_texCoordsVbo);
		glDeleteBuffers(1, &m_tangentsVbo);
	}


	void Plane::upload_tex_coords(float* texCoords, unsigned sizeOfTexCoords)
	{
		bind_geometry();


		// Upload the texture coordinates data
		glBindBuffer(GL_ARRAY_BUFFER, m_texCoordsVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeOfTexCoords, &texCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	}


	void Plane::bind_geometry() const
	{
		glBindVertexArray(m_vao);
	}

	void Plane::unbind_geometry() const
	{
		glBindVertexArray(0);
	}


	void Plane::bind_diffuse() const
	{
		m_diffuse.bind();
	}

	void Plane::unbind_diffuse() const
	{
		m_diffuse.unbind();
	}


	void Plane::bind_normal() const
	{
		m_normal.bind();
	}

	void Plane::unbind_normal() const
	{
		m_normal.unbind();
	}


	int Plane::get_diffuse_texture_unit() const
	{
		return m_diffuse.get_texture_unit();
	}

	int Plane::get_normal_texture_unit() const
	{
		return m_normal.get_texture_unit();
	}


	// Helper function to upload a hardcoded diffuse and normal texture
	void Plane::upload_texture_data()
	{
		// Set the texture units for diffuse and normal
		m_diffuse.set_texture_unit(0);
		m_normal.set_texture_unit(1);

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
		m_normal.bind();

		// Upload the normal texture data
		m_normal.upload_data(normalData, normalInfo);
	}
}