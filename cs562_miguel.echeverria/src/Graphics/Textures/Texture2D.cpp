/**
* @file Texture2D.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Represents a regular 2D texture.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Texture2D.h"
#include <GL/glew.h>


namespace cs460
{
	// Do nothing for now
	Texture2D::Texture2D()
	{
		//std::cout << "Texture2D constructor\n";
	}

	// Do nothing for now
	Texture2D::~Texture2D()
	{
		//std::cout << "Texture2D destructor\n";
	}


	// Load a texture from file and upload the data to the GPU
	void Texture2D::upload_data(const unsigned char* data, const TextureInformation& config)
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


	// Bind and unbind the current texture unit to/from GL_TEXTURE_2D
	void Texture2D::internal_bind() const
	{
		glActiveTexture(GL_TEXTURE0 + get_texture_unit());
		glBindTexture(GL_TEXTURE_2D, get_id());
	}

	void Texture2D::internal_unbind() const
	{
		glActiveTexture(GL_TEXTURE0 + m_texUnit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
