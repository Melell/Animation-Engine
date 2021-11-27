/**
* @file PlayerController.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Script that handles the movement of the player used for the animation
*		 blending demo.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "PlayerController.h"
#include "Platform/InputMgr.h"
#include "Platform/FrameRateController.h"
#include "Composition/SceneNode.h"
#include "Math/Interpolation/InterpolationFunctions.h"


namespace cs460
{
	PlayerControler::PlayerControler()
	{
	}

	PlayerControler::~PlayerControler()
	{
	}


	void PlayerControler::initialize()
	{

	}

	void PlayerControler::update()
	{
		InputMgr& inputMgr = InputMgr::get_instance();
		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();

		const float MOVE_SPEED = 2.0f;
		const glm::vec3 globalUp(0.0f, 1.0f, 0.0f);

		const glm::vec2& leftStick2d = inputMgr.get_gamepad_stick_vec(GAMEPAD::left_stick);
		const glm::vec3& leftStick3d = glm::vec3(leftStick2d.x, 0.0f, leftStick2d.y);
		float stickLength = glm::length(leftStick2d);

		if (stickLength > 0.4f)
		{
			// Get the stick angle in radians
			float stickAngle = glm::degrees(atan2(leftStick2d.y, leftStick2d.x));
			if (stickAngle < 0.0f)
				stickAngle += 360.0f;
			stickAngle = glm::radians(stickAngle + 90.0f);

			// Get the current rotation quaternion and the target rotation quaternion based on the stick input
			glm::quat currQuat = get_owner()->m_localTr.m_orientation;
			glm::quat stickQuat(glm::vec3(0.0f, stickAngle, 0.0f));

			// Slerp the rotation to make it smooth
			glm::quat currOrientation = glm::slerp(currQuat, stickQuat, dt * 6.0f);
			get_owner()->m_localTr.m_orientation = currOrientation;

			// Apply movement in the current forward direction
			m_currentForward = glm::normalize(currOrientation* m_startingForward);
			get_owner()->m_localTr.m_position += m_currentForward * MOVE_SPEED * dt;
		}
	}


	void PlayerControler::on_gui()
	{

	}
}