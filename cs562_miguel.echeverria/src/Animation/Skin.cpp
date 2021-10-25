/**
* @file Skin.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents a skin object from gltf.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Skin.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	void Skin::load_skin_data(const tinygltf::Model& model, int skinIdx, const std::map<int, int>& skinNodes)
	{
		const tinygltf::Skin& skin = model.skins[skinIdx];

		m_name = skin.name;
		m_joints = skin.joints;
		m_commonRootIdx = skin.skeleton > 0 ? skin.skeleton : skinNodes.at(skinIdx);
		load_inv_bind_matrices(model, skin);
	}


	void Skin::load_inv_bind_matrices(const tinygltf::Model& model, const tinygltf::Skin& skin)
	{
		m_invBindMatrices.resize(skin.joints.size());

		const tinygltf::Accessor& accessor = model.accessors[skin.inverseBindMatrices];
		const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		const unsigned char* data = model.buffers[bufferView.buffer].data.data() + bufferView.byteOffset + accessor.byteOffset;

		int stride = accessor.ByteStride(bufferView);

		for (int i = 0; i < m_invBindMatrices.size(); ++i)
			m_invBindMatrices[i] = glm::make_mat4(data + stride * i);
	}
}