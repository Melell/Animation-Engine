/**
* @file Picking.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief In charge of all the picking logic.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once



namespace cs460
{
	class Picking
	{
	public:

		friend class Editor;

		void update();

	private:

		glm::vec3 viewport_to_world_normalized_ray(const glm::vec2& viewportCoords);
		glm::vec3 viewport_to_ndc(const glm::vec2& viewportCoords);
		glm::vec4 ndc_to_homogeneuos_clip_space(const glm::vec3& ndcCoords);
		glm::vec4 homogeneuos_clip_space_to_camera_space(const glm::vec4& clipCoords);
		glm::vec3 camera_to_world_space(const glm::vec4& camCoords);

		Picking();
		Picking(const Picking&) = delete;
		Picking& operator=(const Picking&) = delete;
	};
}