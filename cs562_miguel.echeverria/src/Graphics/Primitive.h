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
	//class MeshRenderable;
	class Shader;


	class Primitive
	{
	public:

		Primitive();
		~Primitive();

		// Process the primitive's data from a tinygltf model's primitive into this one
		void process_primitive_data(const tinygltf::Model& model, const tinygltf::Primitive& primitive);//, MeshRenderable* meshOwner);

		// Draw the primitive
		void render() const;

		Shader* get_shader() const;

		// Free all the opengl buffers used by this primitive
		void delete_gl_buffers();

	private:
		unsigned m_vao = -1;
		std::map<int, unsigned> m_vbos;			// Having a map allows us to generate the vbos on the fly without duplicates instead of having two passes
		//MeshRenderable* m_meshOwner;			// Pointer to the mesh renderable component that owns this primitive
		Shader* m_shader = nullptr;				// Pointer to the shader resource that will be used for drawing this primitive
		size_t m_offset = -1;					// Offset into the vbo/ebo
		size_t m_elementCount = -1;				// The number of elements or vertices to draw (using ebo vs not using it)
		int m_eboComponentType = -1;			// The type of the ebo data (unsigned char, short etc)
		int m_mode = -1;						// GL_POINTS, GL_LINES, GL_TRIANGLES etc
		bool m_usesEbo = false;
		// Material reference (the material should be as a separate resource?
	};
}