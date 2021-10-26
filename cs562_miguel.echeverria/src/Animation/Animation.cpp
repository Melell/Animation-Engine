/**
* @file Animation.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents an animation resource from a gltf model.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Animation.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	void AnimationChannel::load_channel_data(const tinygltf::Animation& anim, int channelIdx)
	{
		const tinygltf::AnimationChannel& channel = anim.channels[channelIdx];
		m_targetProperty = channel.target_path;
		m_targetNodeIdx = channel.target_node;
		m_animDataIdx = channel.sampler;
	}


	void AnimationData::load_keyframe_data(const tinygltf::Model& model, const tinygltf::Animation& anim, int samplerIdx)
	{
		const tinygltf::AnimationSampler& sampler = anim.samplers[samplerIdx];
		m_interpolationMethod = sampler.interpolation;

		load_input_data(model, sampler.input);
		load_output_data(model, sampler.output);
	}

	void AnimationData::load_input_data(const tinygltf::Model& model, int accessorIdx)
	{
		const tinygltf::Accessor& accessor = model.accessors[accessorIdx];
		const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		const unsigned char* data = model.buffers[bufferView.buffer].data.data() + bufferView.byteOffset + accessor.byteOffset;

		// Assume the time values will always be floats, and that they will be contiguous
		m_keys.resize(accessor.count);
		std::memcpy(m_keys.data(), data, bufferView.byteLength);

		//int stride = accessor.ByteStride(bufferView);
		//m_keys.resize(accessor.count);

		// Assume the time values will always be floats
		//for (int i = 0; i < m_keys.size(); ++i)
		//	m_keys[i] = *(reinterpret_cast<const float*>(data + stride * i));
	}

	void AnimationData::load_output_data(const tinygltf::Model& model, int accessorIdx)
	{
		const tinygltf::Accessor& accessor = model.accessors[accessorIdx];
		const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		const unsigned char* data = model.buffers[bufferView.buffer].data.data() + bufferView.byteOffset + accessor.byteOffset;

		// Assume the data will be contiguous
		int componentsInType = tinygltf::GetNumComponentsInType(accessor.type);
		int elementCount = accessor.count;
		int floatCount = elementCount * componentsInType;
		m_values.resize(floatCount);
		std::memcpy(m_values.data(), data, bufferView.byteLength);

		// Assume we are dealing with vec3 or vec4
		//int numberOfComponents = accessor.type;
		//
		//int stride = accessor.ByteStride(bufferView);
		//m_values.resize(accessor.count * numberOfComponents);
		//
		//// For each "attribute" value (vec3, vec4 etc)
		//for (int i = 0; i < accessor.count; ++i)
		//{
		//	// For each float component, add the float
		//	for (int j = 0; j < numberOfComponents; ++j)
		//	{
		//		m_values[i * numberOfComponents + j] = *(reinterpret_cast<const float*>(data + stride * i));
		//	}
		//}
	}


	void Animation::load_animation_data(const tinygltf::Model& model, const tinygltf::Animation& anim)
	{
		m_name = anim.name;

		// Load the channel data
		m_channels.resize(anim.channels.size());
		for (int i = 0; i < m_channels.size(); ++i)
			m_channels[i].load_channel_data(anim, i);


		// Load the samplers data (the keyframe data)
		m_animData.resize(anim.samplers.size());
		for (int i = 0; i < m_animData.size(); ++i)
			m_animData[i].load_keyframe_data(model, anim, i);
	}
}