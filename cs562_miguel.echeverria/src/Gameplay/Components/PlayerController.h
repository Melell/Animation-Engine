/**
* @file PlayerController.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Script that handles the movement of the player used for the animation
*		 blending demo.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/Gameplay/ScriptComponent.h"


namespace cs460
{
	class PlayerControler : public ScriptComponent
	{
	public:

		PlayerControler();
		virtual ~PlayerControler();

		void initialize() override;
		void update() override;

	private:

		const glm::vec3 m_startingForward{ 0.0f, 0.0f, 1.0f };
		glm::vec3 m_currentForward{ 0.0f, 0.0f, 1.0f };

		void on_gui() override;
	};
}