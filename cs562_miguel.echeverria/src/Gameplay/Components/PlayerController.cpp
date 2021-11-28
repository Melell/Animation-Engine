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
#include "Cameras/ICamera.h"
#include "Components/Animation/AnimationReference.h"
#include "Animation/Blending/Blend1D.h"
#include "Animation/Blending/Blend2D.h"


namespace cs460
{
	PlayerController::PlayerController()
	{
	}

	PlayerController::~PlayerController()
	{
	}


	void PlayerController::initialize()
	{

	}

	void PlayerController::update()
	{
		InputMgr& inputMgr = InputMgr::get_instance();
		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();

		// Get the left stick input (camera space)
		const glm::vec2& leftStick2d = inputMgr.get_gamepad_stick_vec(GAMEPAD::left_stick);
		const glm::vec3& leftStick3d = glm::vec3(leftStick2d.x, 0.0f, -leftStick2d.y);
		float stickLength = glm::length(leftStick2d);

		// Get the right trigger input
		float rightTigger = inputMgr.get_gamepad_trigger(GAMEPAD::right_trigger);

		glm::vec2 blendParam(0.0f, 0.0f);

		// Blend parameter controlling
		AnimationReference* animComp = get_owner()->get_component<AnimationReference>();
		Blend1D* blend1DTree = dynamic_cast<Blend1D*>(animComp->m_blendTree);
		Blend2D* blend2DTree = dynamic_cast<Blend2D*>(animComp->m_blendTree);
		if (blend1DTree)
		{
			float prevParam = blend1DTree->m_blendParam;
			float currParam = lerp(prevParam, stickLength, dt * 6.0f);
			blend1DTree->m_blendParam = currParam;

			if (rightTigger > 0.1f && stickLength > 0.4f)
			{
				float sprintParam = lerp(prevParam, 1.0f + rightTigger, dt * 4.0f);
				blend1DTree->m_blendParam = sprintParam;
			}

			blendParam.x = blend1DTree->m_blendParam;

			blend_1d_controls(leftStick3d, stickLength, blendParam.x);
		}
		else if (blend2DTree)
		{
			//glm::vec2 prevParam = blend2DTree->m_blendParam;
			//glm::vec2 currParam = lerp(prevParam, stickLength, dt * 6.0f);
			//blend1DTree->m_blendParam = currParam;
			//
			//if (rightTigger > 0.1f && stickLength > 0.4f)
			//{
			//	float sprintParam = lerp(prevParam, 1.0f + rightTigger, dt * 4.0f);
			//	blend1DTree->m_blendParam = sprintParam;
			//}
			//
			//blendParam.x = blend1DTree->m_blendParam;

			//blend_2d_controls(leftStick3d, stickLength, blendParam);
		}
	}


	void PlayerController::blend_1d_controls(const glm::vec3& leftStickCamSpace, float stickLength, float blendParam)
	{
		Scene& scene = Scene::get_instance();
		ICamera* cam = scene.get_active_camera();

		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();

		const float MAX_SPEED = 5.0f;

		if (stickLength > 0.1f)
		{
			// Transform stick input from camera space to world space
			const glm::mat4& invViewMtx = glm::inverse(cam->get_view_mtx());
			glm::vec3 stickWorldSpace = glm::vec3(invViewMtx * glm::vec4(leftStickCamSpace, 0.0f));
			stickWorldSpace.y = 0.0f;
			stickWorldSpace = glm::normalize(stickWorldSpace) * stickLength;


			// Get the stick angle in radians
			float stickAngle = glm::degrees(atan2(stickWorldSpace.z, stickWorldSpace.x));
			if (stickAngle < 0.0f)
				stickAngle += 360.0f;
			stickAngle = glm::radians(stickAngle - 90);

			// Get the current rotation quaternion and the target rotation quaternion based on the stick input
			glm::quat currQuat = get_owner()->m_localTr.m_orientation;
			glm::quat stickQuat(glm::vec3(0.0f, -stickAngle, 0.0f));

			// Slerp the rotation to make it smooth
			glm::quat currOrientation = glm::normalize(glm::slerp(currQuat, stickQuat, dt * 10.0f));
			get_owner()->m_localTr.m_orientation = currOrientation;

			// Apply movement in the current forward direction

			m_currentForward = glm::normalize(currOrientation * m_startingForward);
			get_owner()->m_localTr.m_position += m_currentForward * MAX_SPEED * blendParam * dt;
		}
	}

	void PlayerController::blend_2d_controls(const glm::vec3& leftStickCamSpace, float stickLength, const glm::vec2& blendParam)
	{
		FrameRateController& frc = FrameRateController::get_instance();
		float dt = frc.get_dt_float();


	}

	
	void PlayerController::on_gui()
	{

	}
}