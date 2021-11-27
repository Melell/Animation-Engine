/**
* @file Joint.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Component that represents a joint in a skin. Merely for showing
*		 the info in the editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"


namespace cs460
{
	class Joint : public IComponent
	{
	public:

		Joint();
		virtual ~Joint();

		// Getters and setters for the skin index
		void set_skin_idx(int idx);
		int get_skin_idx() const;

	private:
		int m_skinIdx = -1;

		void on_gui() override;
	};
}