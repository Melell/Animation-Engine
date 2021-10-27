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

struct GLFWwindow;


namespace cs460
{
	// For animated cameras, the camera class will be made a component.
	class EditorCamera
	{
	public:

		// Logic for the editor camera
		void update();

		// Getters and setters for position and target
		glm::vec3 get_position() const;
		glm::vec3 get_target() const;
		void set_position(const glm::vec3& newPosition);
		void set_target(const glm::vec3& newTarget);

		// Getters for the camera basis vectors
		glm::vec3 get_view_vec() const;
		glm::vec3 get_right_vec() const;
		glm::vec3 get_up_vec() const;

		// Getters for the camera matrices
		glm::mat4 get_view_mtx() const;
		glm::mat4 get_projection_mtx() const;

		float m_movementSpeed = 15.0f;
		float m_keyboardTiltSpeed = 100.0f;
		float m_mouseTiltSpeed = 100.0f;

	private:
		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 25.0f);
		glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 m_viewMtx = glm::mat4(1.0f);
		PerspectiveCamera m_camera;

		float m_initialCursorPosX = 0.0f;
		float m_initialCursorPosY = 0.0f;
		bool m_initialPosRecorded = false;

		// Actual logic for the editor camera goes here
		void camera_controls();

		// Update the view matrix with the current position and target
		void update_view_mtx();

		// Record the mouse x and y screen coordinates.
		void record_cursor_position(GLFWwindow* window);

		// Gets the cursor position in screen coordinates as a float
		void get_cursor_pos(GLFWwindow* window, float& xPos, float& yPos);
	};
}