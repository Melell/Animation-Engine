/**
* @file Skin.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a skin object from gltf.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct Skin
	{
		std::string m_name;
		std::vector<glm::mat4> m_invBindMatrices;
		std::vector<int> m_joints;
		int m_commonRootIdx;
	};
}