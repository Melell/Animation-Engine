/**
* @file BlendTree.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Container for the actual hierarchy of the blend tree.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once



namespace cs460
{
	struct IBlendNode;


	/*class BlendTree : public ISerializable
	{
	public:

		// Default ctor and dtor
		BlendTree();
		~BlendTree();

		void produce_pose(float time);
		AnimPose& get_current_pose();

		IBlendNode* get_root();

		// Returns true if the root is null
		bool empty();

		void clear();

	private:
		IBlendNode* m_root = nullptr;


		void clear_node(IBlendNode* node);

		void internalFromJson(json& value) override;
		void internalToJson(json& value) override;
	};*/
}