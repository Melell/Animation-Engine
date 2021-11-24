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
	struct BlendNode;


	class BlendTree : public ISerializable
	{
	public:

		// Default ctor and dtor
		BlendTree();
		~BlendTree();



	private:
		IBlendNode* m_root;
	};
}