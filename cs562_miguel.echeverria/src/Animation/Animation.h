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
	struct AnimationChannel
	{
		void load_channel_data(const tinygltf::Animation& anim, int channelIdx);


		std::string m_targetProperty;
		int m_targetNodeIdx;
		int m_animDataIdx;
	};

	struct AnimationData
	{
		void load_keyframe_data(const tinygltf::Model& model, const tinygltf::Animation& anim, int samplerIdx);
		void load_input_data(const tinygltf::Model& model, int accessorIdx);
		void load_output_data(const tinygltf::Model& model, int accessorIdx);


		std::vector<float> m_keys;
		std::vector<float> m_values;
		std::string m_interpolationMethod;
	};


	struct Animation
	{
		void load_animation_data(const tinygltf::Model& model, const tinygltf::Animation& anim);


		std::string m_name;
		std::vector<AnimationChannel> m_channels;
		std::vector<AnimationData> m_animData;
	};
}