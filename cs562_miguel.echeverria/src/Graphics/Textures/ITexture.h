/**
* @file ITexture.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Base interface for all types of textures (2d texture, cube map etc).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct TextureInformation
	{
		TextureInformation();

		int m_minFilter;
		int m_magFilter;
		int m_sWrap;
		int m_tWrap;
		int m_rWrap;
		int m_width;
		int m_height;
		int m_format;				// GL_RED, GL_RG, GL_RGB or GL_RGBA
		int m_componentType;		// The data type of each pixel channel (red, blue etc)
		bool m_generateMipMaps;
	};


	class ITexture
	{
	public:
	
		ITexture();					// Generate the texture handle
		virtual ~ITexture();		// Free the texture

		// To create and free the texture manually
		void generate();
		void clear();

		// Getter and setter for the texture unit
		void set_texture_unit(int unit);
		int get_texture_unit() const;

		// Bind and unbind the current texture unit to/from the appropriate
		// target (overriden by each specific texture type)
		void bind() const;
		void unbind() const;

		unsigned get_id() const;

	protected:
		unsigned m_id = 0;
		int m_texUnit = 0;


		// Meant to be overriden by each type of texture with glActiveTexture and glBindTexture
		virtual void internal_bind() const = 0;
		virtual void internal_unbind() const = 0;
	};
}