/**
* @file ImageProcessing.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains some image related useful functions like loading from file, saving to file etc.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ImageProcessing.h"
#include <stb/stb_image.h>


namespace cs460
{
	unsigned char* load_image(const std::string& filePath, int& width, int& height, int& channelsInFile, int desiredChannels, bool flipVerticallyOnLoad)
	{
		stbi_set_flip_vertically_on_load(flipVerticallyOnLoad);
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channelsInFile, desiredChannels);

		if (data == nullptr)
			std::cout << "ERROR: Failed to load texture " << filePath << std::endl;

		return data;
	}

	void free_image(unsigned char* data)
	{
		if (data == nullptr)
			return;

		stbi_image_free(data);
	}
}