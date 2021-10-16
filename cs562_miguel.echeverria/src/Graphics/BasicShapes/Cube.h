/**
* @file Cube.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Represents the geometry of a cube.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Cube
	{
	public:

		friend class ResourceManager;	// Only the resource manager can create cubes (supposed to only exist one)

		~Cube();

		void bind() const;
		void unbind() const;

	private:
		unsigned m_vao = -1;
		unsigned m_vbo = -1;

		Cube();
		Cube(const Cube&) = delete;
		Cube& operator=(const Cube&) = delete;
	};
}