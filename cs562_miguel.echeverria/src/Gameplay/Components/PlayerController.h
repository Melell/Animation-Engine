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
	class PlayerController : public ScriptComponent
	{
	public:

		PlayerController();
		virtual ~PlayerController();

		void initialize() override;
		void update() override;

	private:

		const glm::vec3 m_startingForward{ 0.0f, 0.0f, 1.0f };
		glm::vec3 m_currentForward{ 0.0f, 0.0f, 1.0f };

		// The controls for both ended up being awfully similar, will improve code reuse in the future
		void blend_1d_controls(const glm::vec3& leftStickWorldSpace, float stickLength, float blendParam);
		void blend_2d_controls(const glm::vec3& leftStickWorldSpace, const glm::vec2& param);
		glm::vec2 clamp_parameter_to_romboid(const glm::vec2& param);

		void on_gui() override;
	};
}