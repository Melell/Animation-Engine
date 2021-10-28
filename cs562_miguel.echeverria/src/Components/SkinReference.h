/**
* @file SkinReference.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that indicates this node's mesh is using a skin.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IComponent.h"


namespace cs460
{
	class SkinReference : public IComponent
	{
	public:

		SkinReference();
		virtual ~SkinReference();

		// Getters and setters for the skin index
		void set_skin_idx(int idx);
		int get_skin_idx() const;

		std::vector<glm::mat4>& get_joint_matrices();

	private:
		int m_skinIdx = -1;
		std::vector<glm::mat4> m_jointMatrices;

		void on_gui() override;
	};
}