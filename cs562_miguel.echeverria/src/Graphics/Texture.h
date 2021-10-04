/**
* @file Texture.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Holds the opengl handle and other info for a texture 2d unit.
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
		int m_width;
		int m_height;
		int m_format;				// GL_RED, GL_RG, GL_RGB or GL_RGBA
		int m_componentType;		// The data type of each pixel channel (red, blue etc)
		bool m_generateMipMaps;
	};


	class Texture
	{
	public:
	
		Texture();		// Generate the texture handle
		~Texture();		// Free the texture
		
		// Getter and setter for the texture unit
		void set_texture_unit(int unit);
		int get_texture_unit() const;

		// Bind and unbind the current texture unit to/from GL_TEXTURE_2D
		void bind() const;
		void unbind() const;

		// Upload the texture data to the GPU
		void upload_data(const unsigned char* data, const TextureInformation& config);

		unsigned get_id() const;

	private:
		unsigned m_id = 0;
		int m_texUnit = 0;
		int m_width = 1;
		int m_height = 1;
	};
}