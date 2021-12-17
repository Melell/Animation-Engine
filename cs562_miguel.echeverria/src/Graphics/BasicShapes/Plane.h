/**
* @file Plane.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Basic plane that can be rendered.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Graphics/Textures/Texture2D.h"


namespace cs460
{
	class Plane
	{
	public:

		friend class ResourceManager;	// Only the resource manager can create planes (supposed to only exist one)

		~Plane();

		void bind_geometry() const;
		void unbind_geometry() const;

		void bind_diffuse() const;
		void unbind_diffuse() const;

		void bind_normal() const;
		void unbind_normal() const;

	private:
		unsigned m_vao = -1;
		unsigned m_vbo = -1;
		Texture2D m_diffuse;
		Texture2D m_normal;

		// Helper function to upload a hardcoded diffuse and normal texture
		void upload_texture_data();

		Plane();
		Plane(const Plane&) = delete;
		Plane& operator=(const Plane&) = delete;
	};
}