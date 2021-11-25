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
#include "Math/Interpolation/InterpolationFunctions.h"
#include <gltf/tiny_gltf.h>


namespace cs460
{
	AnimationProperty::AnimationProperty()
		:	m_property(nullptr),
			m_animIdx(0),
			m_animDataIdx(0),
			m_interpolationMode(INTERPOLATION_MODE::LERP)
	{
	}
	


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

		// Save the total time it takes to complete the interpolation of this property
		float maxTime = (float)accessor.maxValues.front();
		if (maxTime > m_time)
			m_time = maxTime;

		int byteStride = accessor.ByteStride(bufferView);
		m_keys.resize(accessor.count);

		// Assume the time values will always be floats
		for (int i = 0; i < m_keys.size(); ++i)
			m_keys[i] = *(reinterpret_cast<const float*>(data + byteStride * i));
	}

	void AnimationData::load_output_data(const tinygltf::Model& model, int accessorIdx)
	{
		const tinygltf::Accessor& accessor = model.accessors[accessorIdx];
		const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
		const unsigned char* data = model.buffers[bufferView.buffer].data.data() + bufferView.byteOffset + accessor.byteOffset;

		// Resize the float vector to the appropriate size
		m_componentCount = tinygltf::GetNumComponentsInType(accessor.type);
		size_t elementCount = accessor.count;
		size_t floatCount = elementCount * m_componentCount;
		m_values.resize(floatCount);

		int componentSize = tinygltf::GetComponentSizeInBytes(accessor.componentType);
		int byteStride = accessor.ByteStride(bufferView);
		int elementSize = componentSize * m_componentCount;

		// For each "element" value (vec3, vec4 etc), copy from the gltf data to our own
		for (int i = 0; i < accessor.count; ++i)
			std::memcpy(m_values.data() + i * m_componentCount, data + i * byteStride, elementSize);
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
		{
			m_animData[i].load_keyframe_data(model, anim, i);

			// Save the time to complete the animation (longest interpolation)
			if (m_animData[i].m_time > m_duration)
				m_duration = m_animData[i].m_time;
		}
	}


	glm::vec3 Animation::sample_vec3(float time, int channelIdx)
	{
		AnimationChannel& channel = m_channels[channelIdx];
		AnimationData& animData = m_animData[channel.m_animDataIdx];

		const std::string& method = animData.m_interpolationMethod;
		const std::string& property = channel.m_targetProperty;
		const std::vector<float>& keys = animData.m_keys;
		const std::vector<float>& values = animData.m_values;
		
		glm::vec3 result{ 0.0f, 0.0f, 0.0f };

		if (method == "LINEAR")
			result = piecewise_lerp(animData.m_keys, animData.m_values, time);
		else if (method == "STEP")
			result = piecewise_step(animData.m_keys, animData.m_values, time);
		else if (method == "CUBICSPLINE")
			result = piecewise_hermite(animData.m_keys, animData.m_values, time);

		return result;
	}

	glm::quat Animation::sample_quat(float time, int channelIdx, bool useNLerp)
	{
		AnimationChannel& channel = m_channels[channelIdx];
		AnimationData& animData = m_animData[channel.m_animDataIdx];

		glm::quat resultQuat = piecewise_slerp(animData.m_keys, animData.m_values, time);
		if (useNLerp)
			resultQuat = glm::normalize(resultQuat);

		return resultQuat;
	}
}