/**
* @file CubeMap.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains 6 2d textures for representing a cubemapped texture.
*		 This can be used for skybox, enviroment mapping etc
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "CubeMap.h"
#include "Utilities/ImageProcessing.h"
#include <GL/glew.h>


namespace cs460
{
	// Does nothing for now
	CubeMap::CubeMap()
	{
		//std::cout << "CubeMap constructor\n";
	}

	// Does nothing for now
	CubeMap::~CubeMap()
	{
		//std::cout << "CubeMap destructor\n";
	}


	// Functions for loading the cubemap textures (if all at once, filePaths should
	// be in correct order according to opengl: +x, -x, +y, -y, +z, -z)
	void CubeMap::load_cubemap_textures(const std::vector<std::string>& filePaths) const
	{
		if (filePaths.size() < 6)
		{
			std::cout << "ERROR: Less than 6 textures provided to the cubemap\n";
			return;
		}

		load_texture_pos_x(filePaths[0]);
		load_texture_neg_x(filePaths[1]);
		load_texture_pos_y(filePaths[2]);
		load_texture_neg_y(filePaths[3]);
		load_texture_pos_z(filePaths[4]);
		load_texture_neg_z(filePaths[5]);
	}

	void CubeMap::load_texture_pos_x(const std::string& filePath) const
	{
		int width, height, channelsInFile;
		unsigned char* data = load_image(filePath, width, height, channelsInFile, 3, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free_image(data);
	}

	void CubeMap::load_texture_neg_x(const std::string& filePath) const
	{
		int width, height, channelsInFile;
		unsigned char* data = load_image(filePath, width, height, channelsInFile, 3, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free_image(data);
	}

	void CubeMap::load_texture_pos_y(const std::string& filePath) const
	{
		int width, height, channelsInFile;
		unsigned char* data = load_image(filePath, width, height, channelsInFile, 3, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free_image(data);
	}

	void CubeMap::load_texture_neg_y(const std::string& filePath) const
	{
		int width, height, channelsInFile;
		unsigned char* data = load_image(filePath, width, height, channelsInFile, 3, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free_image(data);
	}

	void CubeMap::load_texture_pos_z(const std::string& filePath) const
	{
		int width, height, channelsInFile;
		unsigned char* data = load_image(filePath, width, height, channelsInFile, 3, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free_image(data);
	}

	void CubeMap::load_texture_neg_z(const std::string& filePath) const
	{
		int width, height, channelsInFile;
		unsigned char* data = load_image(filePath, width, height, channelsInFile, 3, false);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free_image(data);
	}

	void CubeMap::set_texture_parameters() const
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}


	// Bind and unbind the current texture unit to/from GL_TEXTURE_CUBE_MAP
	void CubeMap::internal_bind() const
	{
		glActiveTexture(GL_TEXTURE0 + get_texture_unit());
		glBindTexture(GL_TEXTURE_CUBE_MAP, get_id());
	}
	void CubeMap::internal_unbind() const
	{
		glActiveTexture(GL_TEXTURE0 + get_texture_unit());
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}