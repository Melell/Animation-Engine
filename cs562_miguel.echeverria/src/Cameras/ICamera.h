/**
* @file ICamera.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Base class for all cameras in the game/editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class ICamera
	{
	public:

		// Call the internal virtual method that handles the camera controls
		void update();

		// Getter and setter for the current position of the camera
		const glm::vec3& get_position() const;
		void set_position(const glm::vec3 pos);

		// Meant to be implemented by child classes(editor cam, spherical cam etc)
		virtual void update_view_mtx() = 0;
		const glm::mat4& get_view_mtx() const;

		// Meant to be implemented by child classes (perspective and orthogonal cams)
		virtual void update_projection_mtx() = 0;
		const glm::mat4& get_projection_mtx() const;

		// Setter and getter for the boolean that determines whether the camera is updated or not
		void set_is_active(bool isActive);
		bool get_is_active() const;

	protected:
		glm::vec3 m_position = glm::vec3(0.0f, 1.0f, 25.0f);
		glm::mat4 m_viewMtx = glm::mat4(1.0f);
		glm::mat4 m_projectionMtx = glm::mat4(1.0f);

		bool m_isActive = true;


		// Meant to be overriden by specific cameras
		virtual void camera_logic() = 0;
	};
}