/**
* @file Primitive.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Resource that holds handles to opengl buffers, a material, and
*		 all the necessary data to be drawn.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

namespace tinygltf
{
	class Model;
	struct Primitive;
}


namespace cs460
{
	class Primitive
	{
	public:

		Primitive();
		~Primitive();

		// Process the primitive's data from a tinygltf model's primitive into this one
		void process_primitive_data(const tinygltf::Model& model, const tinygltf::Primitive& primitive);

		// Draw the primitive
		void render() const;

		// Free all the opengl buffers used by this primitive
		void delete_gl_buffers();

	private:
		unsigned m_vao = -1;
		std::vector<unsigned> m_vbos;
		int m_mode = -1;						// GL_POINTS, GL_LINES, GL_TRIANGLES etc
		size_t m_offset = -1;
		size_t m_elementCount = -1;
		int m_eboComponentType = -1;
		bool m_usesEbo = false;
		// Material reference (the material should be as a separate resource?
	};
}