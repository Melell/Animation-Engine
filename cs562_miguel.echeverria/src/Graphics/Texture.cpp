/**
* @file Texture.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Holds the opengl handle and other info for a texture 2d unit.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Texture.h"
#include <GL/glew.h>


namespace cs460
{
	TextureInformation::TextureInformation()
		:	m_minFilter(GL_LINEAR_MIPMAP_LINEAR),
			m_magFilter(GL_LINEAR),
			m_sWrap(GL_REPEAT),
			m_tWrap(GL_REPEAT),
			m_width(1),
			m_height(1),
			m_format(GL_RGBA),
			m_componentType(GL_UNSIGNED_BYTE),
			m_generateMipMaps(true)
	{
	}


	// Generate the texture handle
	Texture::Texture()
	{
		glGenTextures(1, &m_id);
	}

	// Free the texture
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_id);
	}

	// Setter for the texture unit
	void Texture::set_texture_unit(int unit)
	{
		m_texUnit = unit;
	}

	// Getter for the texture unit
	int Texture::get_texture_unit() const
	{
		return m_texUnit;
	}

	// Bind the current texture unit to GL_TEXTURE_2D
	void Texture::bind() const
	{
		glActiveTexture(GL_TEXTURE0 + m_texUnit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	// Unbind the current texture unit from GL_TEXTURE_2D
	void Texture::unbind() const
	{
		glActiveTexture(GL_TEXTURE0 + m_texUnit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Load a texture from file and upload the data to the GPU
	void Texture::upload_data(const unsigned char* data, const TextureInformation& config)
	{
		// Bind the texture to the currently set unit
		bind();

		// Set the necessary texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.m_minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.m_magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.m_sWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.m_tWrap);

		// Upload the data to the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config.m_width, config.m_height, 0, GL_RGBA, config.m_componentType, data);

		m_width = config.m_width;
		m_height = config.m_height;

		if (config.m_generateMipMaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	unsigned Texture::get_id() const
	{
		return m_id;
	}
}
