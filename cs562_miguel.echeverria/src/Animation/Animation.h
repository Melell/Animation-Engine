/**
* @file Animation.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents an animation resource from a gltf model.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

namespace tinygltf
{
	class Model;
	struct Animation;
}


namespace cs460
{
	enum class INTERPOLATION_MODE
	{
		LERP,
		SLERP,
		STEP,
		CUBIC_SPLINE
	};


	// Holds a pointer to the property to interpolate, as well as a reference to
	// the resource with the keyframe data, and the function used to interpolate
	struct AnimationProperty
	{
		AnimationProperty();

		float* m_property;
		int m_animIdx;
		int m_animDataIdx;
		INTERPOLATION_MODE m_interpolationMode;
	};


	// Contains info about which node, and which property to animate
	struct AnimationChannel
	{
		void load_channel_data(const tinygltf::Animation& anim, int channelIdx);


		std::string m_targetProperty;
		int m_targetNodeIdx;
		int m_animDataIdx;
	};
	
	// Contains the keyframe data for different time values
	struct AnimationData
	{
		void load_keyframe_data(const tinygltf::Model& model, const tinygltf::Animation& anim, int samplerIdx);
		void load_input_data(const tinygltf::Model& model, int accessorIdx);
		void load_output_data(const tinygltf::Model& model, int accessorIdx);


		std::vector<float> m_keys;
		std::vector<float> m_values;
		std::string m_interpolationMethod;
		int m_componentCount;				// The number of float components for each m_key (I will probably get rid of this in the future)
		float m_time = 0.0f;
	};


	// Represents an animation resource
	struct Animation
	{
		void load_animation_data(const tinygltf::Model& model, const tinygltf::Animation& anim);

		glm::vec3 sample_vec3(float time, int channelIdx);
		glm::quat sample_quat(float time, int channelIdx, bool useNLerp = false);

		std::string m_name;
		std::vector<AnimationChannel> m_channels;
		std::vector<AnimationData> m_animData;
		float m_duration = 0.0f;
	};
}