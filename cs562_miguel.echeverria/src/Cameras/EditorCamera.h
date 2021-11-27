/**
* @file EditorCamera.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the necessary data to form a view matrix. Also has the
*		 editor camera logic.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "PerspectiveCamera.h"


namespace cs460
{
	// For animated cameras, the camera class will be made a component.
	class EditorCamera : public PerspectiveCamera
	{
	public:

		// Update the view matrix with the current position and target
		void update_view_mtx() override;

		// Getter and setter for the target
		glm::vec3 get_target() const;
		void set_target(const glm::vec3& newTarget);

		// Getters for the camera basis vectors
		glm::vec3 get_view_vec() const;
		glm::vec3 get_right_vec() const;
		glm::vec3 get_up_vec() const;


		float m_movementSpeed = 15.0f;
		float m_keyboardTiltSpeed = 100.0f;
		float m_mouseTiltSpeed = 100.0f;

	protected:
		// Actual logic for the editor camera goes here
		void camera_logic() override;

	private:
		glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);

		float m_initialCursorPosX = 0.0f;
		float m_initialCursorPosY = 0.0f;
		bool m_initialPosRecorded = false;

		// Record the mouse x and y screen coordinates.
		void record_cursor_position();

		// Gets the cursor position in screen coordinates as a float
		void get_cursor_pos(float& xPos, float& yPos);
	};
}