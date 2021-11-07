/**
* @file EditorCamera.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the necessary data to form a view matrix. Also has the
*		 editor camera logic.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/


#include "pch.h"
#include "EditorCamera.h"
#include "Platform/FrameRateController.h"
#include "Platform/InputMgr.h"
#include "Graphics/Systems/Renderer.h"		// For getting window width and height


namespace cs460
{
	// Logic for the editor camera
	void EditorCamera::update()
	{
		camera_controls();
		update_view_mtx();
	}


	// Getters for position and target
	glm::vec3 EditorCamera::get_position() const
	{
		return m_position;
	}

	glm::vec3 EditorCamera::get_target() const
	{
		return m_target;
	}

	void EditorCamera::set_position(const glm::vec3& newPosition)
	{
		m_position = newPosition;
	}

	void EditorCamera::set_target(const glm::vec3& newTarget)
	{
		m_target = newTarget;
	}


	// Getters for the camera basis vectors
	glm::vec3 EditorCamera::get_view_vec() const
	{
		return glm::normalize(m_target - m_position);
	}

	glm::vec3 EditorCamera::get_right_vec() const
	{
		const glm::vec3& viewVec = m_target - m_position;
		const glm::vec3& globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
		return glm::normalize(glm::cross(viewVec, globalUp));
	}

	glm::vec3 EditorCamera::get_up_vec() const
	{
		const glm::vec3& viewVec = m_target - m_position;
		const glm::vec3& globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3& rightVec = glm::cross(viewVec, globalUp);
		return glm::normalize(glm::cross(rightVec, viewVec));
	}


	glm::mat4 EditorCamera::get_view_mtx() const
	{
		return m_viewMtx;
	}

	glm::mat4 EditorCamera::get_projection_mtx() const
	{
		return m_camera.get_perspective_mtx();
	}


	// Actual logic for the editor camera goes here
	void EditorCamera::camera_controls()
	{
		FrameRateController& frc = FrameRateController::get_instance();
		Renderer& renderer = Renderer::get_instance();
		InputMgr& inputMgr = InputMgr::get_instance();

		glm::vec3 displacement;

		// Only move the camera with keyboard, if we are not using the keyboard in imgui
		if (!ImGui::GetIO().WantCaptureKeyboard)
		{
			// Lateral movement
			if (inputMgr.is_key_down(KEYS::key_d))
			{
				displacement = get_right_vec() * m_movementSpeed * frc.get_dt_float();
				set_position(get_position() + displacement);
				set_target(get_target() + displacement);
			}
			else if (inputMgr.is_key_down(KEYS::key_a))
			{
				displacement = -get_right_vec() * m_movementSpeed * frc.get_dt_float();
				set_position(get_position() + displacement);
				set_target(get_target() + displacement);
			}

			// Movement along view vec
			if (inputMgr.is_key_down(KEYS::key_w))
			{
				displacement = get_view_vec() * m_movementSpeed * frc.get_dt_float();
				set_position(get_position() + displacement);
				set_target(get_target() + displacement);
			}
			else if (inputMgr.is_key_down(KEYS::key_s))
			{
				displacement = -get_view_vec() * m_movementSpeed * frc.get_dt_float();
				set_position(get_position() + displacement);
				set_target(get_target() + displacement);
			}

			// Movement along global up/down
			glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
			if (inputMgr.is_key_down(KEYS::key_e))
			{
				displacement = globalUp * m_movementSpeed * frc.get_dt_float();
				set_position(get_position() + displacement);
				set_target(get_target() + displacement);
			}
			else if (inputMgr.is_key_down(KEYS::key_q))
			{
				displacement = -globalUp * m_movementSpeed * frc.get_dt_float();
				set_position(get_position() + displacement);
				set_target(get_target() + displacement);
			}

			// Movement of the view vector itself (tilting camera around with keyboard)
			if (inputMgr.is_key_down(KEYS::key_right))
				set_target(get_target() + get_right_vec() * m_keyboardTiltSpeed * frc.get_dt_float());
			if (inputMgr.is_key_down(KEYS::key_left))
				set_target(get_target() - get_right_vec() * m_keyboardTiltSpeed * frc.get_dt_float());
			if (inputMgr.is_key_down(KEYS::key_up))
				set_target(get_target() + get_up_vec() * m_keyboardTiltSpeed * frc.get_dt_float());
			if (inputMgr.is_key_down(KEYS::key_down))
				set_target(get_target() - get_up_vec() * m_keyboardTiltSpeed * frc.get_dt_float());

			// Reset the camera
			if (inputMgr.is_key_pressed(KEYS::key_v))
			{
				set_position(glm::vec3(0.0f, 0.0f, 25.0f));
				set_target(glm::vec3(0.0f, 0.0f, 0.0f));
			}
		}
		

		// Only use mouse for moving camera around if imgui is not using mouse
		if (!ImGui::GetIO().WantCaptureMouse)
		{
			// Only allow mouse panning when right click is held
			if (inputMgr.is_mouse_button_down(MOUSE::button_right))
			{
				// Record the initial position of the cursor for the panning (only once)
				if (!m_initialPosRecorded)
					record_cursor_position();

				// Not ideal way to do it
				// Movement of the view vector itself (panning around with mouse)
				float xCursorPos, yCursorPos;
				get_cursor_pos(xCursorPos, yCursorPos);
				const glm::vec3& rightVec = get_right_vec();
				const glm::vec3& upVec = get_up_vec();
				float rightDisplacement = ((xCursorPos - m_initialCursorPosX) / renderer.get_window().get_window_width()) * m_mouseTiltSpeed;
				float upDisplacement = (-(yCursorPos - m_initialCursorPosY) / renderer.get_window().get_window_height()) * m_mouseTiltSpeed;	// Make up for	GLFW positive downwards y axis
				const glm::vec3& totalDisplacement = rightDisplacement * rightVec + upDisplacement * upVec;

				set_target(get_target() + totalDisplacement);

				record_cursor_position();
			}
			else
				m_initialPosRecorded = false;
		}
	}

	void EditorCamera::update_view_mtx()
	{
		m_viewMtx = glm::lookAt(m_position, m_target, get_up_vec());
	}

	// Record the mouse x and y screen coordinates.
	void EditorCamera::record_cursor_position()
	{
		get_cursor_pos(m_initialCursorPosX, m_initialCursorPosY);
		m_initialPosRecorded = true;
		//std::cout << "POSITION = (" << m_initialCursorPosX << ", " << m_initialCursorPosY << ")\n";
	}

	// Gets the cursor position in screen coordinates as a float
	void EditorCamera::get_cursor_pos(float& xPos, float& yPos)
	{
		const glm::vec2& cursorPos = InputMgr::get_instance().get_mouse_position();
		xPos = static_cast<float>(cursorPos.x);
		yPos = static_cast<float>(cursorPos.y);
	}
}