/**
* @file SkeletonRoot.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that represents the common root of joints in a skin. Merely
*		 for displaying this information in the editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"


namespace cs460
{
	class SkeletonRoot : public IComponent
	{
	public:

		SkeletonRoot();
		virtual ~SkeletonRoot();

		// Getters and setters for the skin index
		void set_skin_idx(int idx);
		int get_skin_idx() const;

	private:
		int m_skinIdx = -1;

		void on_gui() override;
	};
}