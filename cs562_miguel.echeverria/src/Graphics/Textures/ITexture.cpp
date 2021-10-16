/**
* @file ITexture.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Base interface for all types of textures (2d texture, cube map etc).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ITexture.h"
#include "CubeMap.h"
#include <GL/glew.h>


namespace cs460
{
	TextureInformation::TextureInformation()
		:	m_minFilter(GL_LINEAR_MIPMAP_LINEAR),
			m_magFilter(GL_LINEAR),
			m_sWrap(GL_REPEAT),
			m_tWrap(GL_REPEAT),
			m_rWrap(GL_REPEAT),
			m_width(1),
			m_height(1),
			m_format(GL_RGBA),
			m_componentType(GL_UNSIGNED_BYTE),
			m_generateMipMaps(true)
	{
	}


	// Generate the texture handle
	ITexture::ITexture()
	{
		//std::cout << "ITexture constructor\n";
		generate();
	}

	// Free the texture
	ITexture::~ITexture()
	{
		//if (dynamic_cast<CubeMap*>(this))
		//	std::cout << "Cube map destructor called\n";
		//std::cout << "ITexture destructor\n";

		clear();
	}

	// To create and free the texture manually
	void ITexture::generate()
	{
		glGenTextures(1, &m_id);
	}
	
	void ITexture::clear()
	{
		glDeleteTextures(1, &m_id);
	}

	// Setter for the texture unit
	void ITexture::set_texture_unit(int unit)
	{
		m_texUnit = unit;
	}

	// Getter for the texture unit
	int ITexture::get_texture_unit() const
	{
		return m_texUnit;
	}

	// Bind the current texture unit to the appropriate target
	void ITexture::bind() const
	{
		this->internal_bind();
	}

	// Unbind the current texture unit from the appropriate target
	void ITexture::unbind() const
	{
		this->internal_unbind();
	}

	unsigned ITexture::get_id() const
	{
		return m_id;
	}
}