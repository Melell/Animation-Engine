/**
* @file Texture2D.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Represents a regular 2D texture.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "ITexture.h"


namespace cs460
{
	class Texture2D : public ITexture
	{
	public:
	
		Texture2D();
		virtual ~Texture2D();

		// Upload the texture data to the GPU
		void upload_data(const unsigned char* data, const TextureInformation& config);

	private:
		int m_width = 1;
		int m_height = 1;

		// Bind and unbind the current texture unit to/from GL_TEXTURE_2D
		void internal_bind() const override;
		void internal_unbind() const override;
	};
}