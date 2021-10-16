/**
* @file ImageProcessing.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains some image related useful functions like loading from file, saving to file etc.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	unsigned char* load_image(const std::string& filePath, int& width, int& height, int& channelsInFile, int desiredChannels, bool flipVerticallyOnLoad = true);
	void free_image(unsigned char* data);
}