/**
* @file IKChain.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Contains the inverse kinematics chain of nodes.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class SceneNode;


	struct IKChain
	{
	public:

		IKChain(SceneNode* chainRoot = nullptr, SceneNode* endEffector = nullptr, SceneNode* target = nullptr);

		void set_chain_root(SceneNode* chainRoot);
		void set_end_effector(SceneNode* endEffector);
		void set_target(SceneNode* target);

		SceneNode* get_chain_root();
		SceneNode* get_end_effector();
		SceneNode* get_target();

		// Functions to add/remove a joint to/from the end of the chain, changing the end effector
		void push_joint();
		void pop_joint();

	private:
		SceneNode* m_chainRoot = nullptr;
		SceneNode* m_endEffector = nullptr;
		SceneNode* m_target = nullptr;
	};
}