/**
* @file GLTFScene.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Similar to a tinygltf::Scene, holds the indices of the parent nodes in the scene.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "GLTFScene.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	// Loads the data from the gltf scene to our own
	void GLTFScene::load_scene_data(const tinygltf::Model& model, const tinygltf::Scene& scene)
	{
		m_name = scene.name;
		m_nodeIndices = scene.nodes;
	}
}