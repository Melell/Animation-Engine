/**
* @file CubeMap.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains 6 2d textures for representing a cubemapped texture.
*		 This can be used for skybox, enviroment mapping etc
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "ITexture.h"


namespace cs460
{
	class CubeMap : public ITexture
	{
	public:

		CubeMap();
		virtual ~CubeMap();

		// Functions for loading the cubemap textures (if all at once, filePaths should
		// be in correct order according to opengl: +x, -x, +y, -y, +z, -z)
		void load_cubemap_textures(const std::vector<std::string>& filePaths) const;
		void load_texture_pos_x(const std::string& filePath) const;
		void load_texture_neg_x(const std::string& filePath) const;
		void load_texture_pos_y(const std::string& filePath) const;
		void load_texture_neg_y(const std::string& filePath) const;
		void load_texture_pos_z(const std::string& filePath) const;
		void load_texture_neg_z(const std::string& filePath) const;

		void set_texture_parameters() const;

	private:

		// Bind and unbind the current texture unit to/from GL_TEXTURE_2D
		void internal_bind() const override;
		void internal_unbind() const override;
	};
}