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

#include "Graphics/Rendering/Material.h"


namespace tinygltf
{
	class Model;
	struct Primitive;
	struct Material;
	struct Accessor;
	struct BufferView;
	struct Texture;
	struct Image;
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


		// Load the primitive's data from a tinygltf model's primitive into this one
		void load_primitive_data(const tinygltf::Model& model, const tinygltf::Primitive& primitive);//, MeshRenderable* meshOwner);

		// Load the material data from the given tinygltf model to this primitive's material
		void load_material_data(const tinygltf::Model& model, const tinygltf::Material& material);


		// Draw the primitive
		void render() const;


		// Set the shader this primitive will use for drawing (from its name key) and returns it
		Shader* set_shader(const std::string& shaderId);

		// Getters for the shader and material
		Shader* get_shader() const;
		const Material& get_material() const;

		glm::vec3 get_min_pos() const;
		glm::vec3 get_max_pos() const;

		// Free all the opengl buffers used by this primitive
		void delete_gl_buffers();

	private:
		unsigned m_vao = -1;
		std::map<int, unsigned> m_vbos;			// Having a map allows us to generate the vbos on the fly without duplicates instead of having two passes
		//MeshRenderable* m_meshOwner;			// Pointer to the mesh renderable component that owns this primitive
		Shader* m_shader = nullptr;				// Pointer to the shader resource that will be used for drawing this primitive
		Material m_material;
		size_t m_offset = -1;					// Offset into the vbo/ebo
		size_t m_elementCount = -1;				// The number of elements or vertices to draw (using ebo vs not using it)
		int m_eboComponentType = -1;			// The type of the ebo data (unsigned char, short etc)
		int m_mode = -1;						// GL_POINTS, GL_LINES, GL_TRIANGLES etc
		bool m_usesEbo = false;

		// For mesh bv computation
		glm::vec3 m_minPos{  FLT_MAX,  FLT_MAX,  FLT_MAX };
		glm::vec3 m_maxPos{ -FLT_MAX, -FLT_MAX, -FLT_MAX };


		// Save the necessary variables that are needed for drawing, and load
		// the ebo into one of the elements in m_vbos if it uses ebo.
		void setup_ebo(const tinygltf::Model& model, const tinygltf::Primitive& primitive);

		// Get the vertex attribute index that corresponds to the given attribute name.
		int get_attribute_index(const std::string& attributeName);

		// Get the position attibutes' min and max values from its accessor and store those values in m_minPos and m_maxPos
		void get_bounding_values(const tinygltf::Accessor& posAccessor);

		// Create and upload the data of the vbo with the given index, if it hasn't been already created.
		void setup_vbo(const tinygltf::Model& model, const tinygltf::Accessor& accessor, const tinygltf::BufferView& bufferView);

		// Tell OpenGL how to use the vertex attribute given by accessor.
		void setup_vertex_attribute(int attArrayIdx, const tinygltf::Accessor& accessor, const tinygltf::BufferView& bufferView);

		// Set the texture configuration parameters (return them on config parameter) for the texture given by texture and image.
		void set_texture_config(TextureInformation& config, const tinygltf::Model& model, const tinygltf::Texture& texture, const tinygltf::Image& image);
	};
}