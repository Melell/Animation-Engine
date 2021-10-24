/**
* @file Primitive.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Resource that holds handles to opengl buffers, a material, and
*		 all the necessary data to be drawn.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Primitive.h"
//#include "Components/MeshRenderable.h"
#include "Shader.h"
#include "Resources/ResourceManager.h"
#include <gltf/tiny_gltf.h>
#include <GL/glew.h>


namespace cs460
{
	Primitive::Primitive()
	{
		m_shader = ResourceManager::get_instance().get_shader("phong_color");
	}


	Primitive::~Primitive()
	{
		delete_gl_buffers();
	}


	// Process the primitive's data from a tinygltf model's primitive into this one
	void Primitive::load_primitive_data(const tinygltf::Model& model, const tinygltf::Primitive& primitive)//, MeshRenderable* meshOwner)
	{
		//m_meshOwner = meshOwner;

		// Generate and bind the vertex array object
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		
		// Setup the ebo if it uses ebo, and initialize all
		// the variables necessary for drawing later on
		setup_ebo(model, primitive);
		

		// For every attribute in this primitive
		for (auto it = primitive.attributes.begin(); it != primitive.attributes.end(); ++it)
		{
			// Get its accessor
			const std::string& attName = it->first;
			int attIdx = it->second;
			const tinygltf::Accessor& attAccessor = model.accessors[attIdx];
			
			// Get the buffer view(vbo) the current accessor refers to
			const tinygltf::BufferView& bufView = model.bufferViews[attAccessor.bufferView];
			if (bufView.target == GL_ELEMENT_ARRAY_BUFFER)
				continue;


			// Select the location for the current vertex attribute
			int attArrayIdx = get_attribute_index(attName);
			if (attArrayIdx < 0)
				continue;


			// If we haven't created the buffer and uploaded the data yet, do it (this
			// is because we might have only 1 vbo in the case of interleaved data)
			setup_vbo(model, attAccessor, bufView);
			

			// Tell opengl to use the data given by accessor as the vertex attribute array at location attArrayIdx
			setup_vertex_attribute(attArrayIdx, attAccessor, bufView);
		}

		// Process all the material data (color, textures etc)
		const tinygltf::Material& material = model.materials[primitive.material];
		load_material_data(model, material);

		glBindVertexArray(0);
	}


	// Process the material data from the given tinygltf model to this primitive's material
	void Primitive::load_material_data(const tinygltf::Model& model, const tinygltf::Material& material)
	{
		m_material.m_usesBaseTexture = false;
		m_material.m_baseColor.x = (float)material.pbrMetallicRoughness.baseColorFactor[0];
		m_material.m_baseColor.y = (float)material.pbrMetallicRoughness.baseColorFactor[1];
		m_material.m_baseColor.z = (float)material.pbrMetallicRoughness.baseColorFactor[2];
		m_material.m_baseColor.w = (float)material.pbrMetallicRoughness.baseColorFactor[3];
		// Actually, currently not using alpha in the shaders
		
		// BASE TEXTURE
		// If it has a base texture, save it
		const tinygltf::TextureInfo& baseTexInfo = material.pbrMetallicRoughness.baseColorTexture;
		if (baseTexInfo.index >= 0)
		{
			// Set the texture unit and bind the texture
			m_material.m_usesBaseTexture = true;
			m_material.m_baseColorTex.set_texture_unit(0);//baseTexInfo.texCoord);
			m_material.m_baseColorTex.bind();

			// Get the tinygltf Texture, Image and Sampler classes
			const tinygltf::Texture& texture = model.textures[baseTexInfo.index];
			const tinygltf::Image& image = model.images[texture.source];

			// Set the texture appropriate texture parameters
			TextureInformation config;
			set_texture_config(config, model, texture, image);

			// Finally, setup the texture parameters and upload the data to the GPU
			m_material.m_baseColorTex.upload_data(image.image.data(), config);
		}


		// NORMAL MAP TEXTURE
		// If it has a normal map texture, save it
		const tinygltf::NormalTextureInfo& normalTexInfo = material.normalTexture;
		m_material.m_normalMapScale = (float)normalTexInfo.scale;
		if (normalTexInfo.index >= 0)
		{
			// Set the texture unit and bind the texture
			m_material.m_usesNormalTexture = true;
			m_material.m_normalMapTex.set_texture_unit(1);//normalTexInfo.texCoord);
			m_material.m_normalMapTex.bind();

			// Get the tinygltf Texture, Image and Sampler classes
			const tinygltf::Texture& texture = model.textures[normalTexInfo.index];
			const tinygltf::Image& image = model.images[texture.source];

			// Set the texture appropriate texture parameters
			TextureInformation config;
			set_texture_config(config, model, texture, image);

			// Finally, setup the texture parameters and upload the data to the GPU
			m_material.m_normalMapTex.upload_data(image.image.data(), config);
		}
	}


	// Draw the primitive
	void Primitive::render() const
	{
		if (m_material.m_usesBaseTexture)
			m_material.m_baseColorTex.bind();

		if (m_material.m_usesNormalTexture)
			m_material.m_normalMapTex.bind();

		glBindVertexArray(m_vao);

		if (m_usesEbo)
			glDrawElements(m_mode, (GLsizei)m_elementCount, m_eboComponentType, (void*)m_offset);
		else
			glDrawArrays(m_mode, 0, (GLsizei)m_elementCount);

		glBindVertexArray(0);
	}


	// Set the shader this primitive will use for drawing (from its name key) and returns it
	Shader* Primitive::set_shader(const std::string& shaderId)
	{
		// Don't set a new shader if it wasn't found
		Shader* newShader = ResourceManager::get_instance().get_shader(shaderId);
		if (newShader == nullptr)
			return m_shader;

		m_shader = newShader;
		return m_shader;
	}

	Shader* Primitive::get_shader() const
	{
		return m_shader;
	}

	const Material& Primitive::get_material() const
	{
		return m_material;
	}

	// Free all the opengl buffers used by this primitive
	void Primitive::delete_gl_buffers()
	{
		glDeleteVertexArrays(1, &m_vao);

		for(auto it = m_vbos.begin(); it != m_vbos.end(); ++it)
			glDeleteBuffers(1, &it->second);
	}



	// Save the necessary variables that are needed for drawing, and load
	// the ebo into one of the elements in m_vbos if it uses ebo.
	void Primitive::setup_ebo(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
	{
		// Store the necessary variables for drawing later on
		m_mode = primitive.mode == -1 ? GL_TRIANGLES : primitive.mode;
		if (primitive.indices >= 0)
		{
			// Generate the ebo buffer in advance

			const tinygltf::Accessor& idxAccessor = model.accessors[primitive.indices];
			m_offset = idxAccessor.byteOffset;
			m_elementCount = idxAccessor.count;
			m_eboComponentType = idxAccessor.componentType;
			m_usesEbo = true;

			const tinygltf::BufferView& bufView = model.bufferViews[idxAccessor.bufferView];

			// Prepare the ebo
			unsigned ebo;
			glGenBuffers(1, &ebo);
			glBindBuffer(bufView.target, ebo);
			m_vbos[idxAccessor.bufferView] = ebo;

			// Upload the ebo data
			const std::vector<unsigned char>& buffer = model.buffers[bufView.buffer].data;
			glBufferData(bufView.target, bufView.byteLength, buffer.data() + bufView.byteOffset, GL_STATIC_DRAW);
		}
		else
		{
			m_elementCount = model.bufferViews[0].byteLength;
			m_usesEbo = false;
		}
	}


	// Get the vertex attribute index that corresponds to the given attribute name.
	int Primitive::get_attribute_index(const std::string& attributeName)
	{
		// Select the location for the current attribute
		int attArrayIdx = 0;
		if (attributeName == "POSITION")
			attArrayIdx = 0;
		else if (attributeName == "NORMAL")
			attArrayIdx = 1;
		else if (attributeName == "TEXCOORD_0")// + std::to_string(material.pbrMetallicRoughness.baseColorTexture.texCoord))
			attArrayIdx = 2;
		else if (attributeName == "TANGENT")
			attArrayIdx = 3;
		else if (attributeName == "JOINTS_0")
			attArrayIdx = 4;
		else if (attributeName == "WEIGHTS_0")
			attArrayIdx = 5;
		else
			return -1;

		return attArrayIdx;
	}


	// Create and upload the data of the vbo with the given index, if it hasn't been already created.
	void Primitive::setup_vbo(const tinygltf::Model& model, const tinygltf::Accessor& accessor, const tinygltf::BufferView& bufferView)
	{
		// If we haven't created the buffer and uploaded the data yet, do it (this
		// is because we might have only 1 vbo in the case of interleaved data)
		if (m_vbos.find(accessor.bufferView) == m_vbos.end())
		{
			// Prepare the vbo
			unsigned vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(bufferView.target, vbo);
			m_vbos[accessor.bufferView] = vbo;

			// Upload the vbo data
			const std::vector<unsigned char>& buffer = model.buffers[bufferView.buffer].data;
			glBufferData(bufferView.target, bufferView.byteLength, buffer.data() + bufferView.byteOffset, GL_STATIC_DRAW);
		}
	}


	// Tell OpenGL how to use the vertex attribute given by accessor.
	void Primitive::setup_vertex_attribute(int attArrayIdx, const tinygltf::Accessor& accessor, const tinygltf::BufferView& bufferView)
	{
		// Get the number of components in each vertex attribute
		int attComponentCount = 1;
		if (accessor.type != TINYGLTF_TYPE_SCALAR)
			attComponentCount = accessor.type;


		// Get the stride of the attribute, and whether it is normalized
		int stride = accessor.ByteStride(bufferView);
		GLboolean normalized = accessor.normalized ? GL_TRUE : GL_FALSE;

		// Bind the vbo this attribute refers to, and tell OpenGL how to interpret the attribute data within the vbo
		glBindBuffer(bufferView.target, m_vbos[accessor.bufferView]);
		glEnableVertexAttribArray(attArrayIdx);
		glVertexAttribPointer(attArrayIdx, attComponentCount, accessor.componentType, normalized, stride, (void*)accessor.byteOffset);
	}


	// Set the texture configuration parameters (return them on config parameter) for the texture given by texture and image.
	void Primitive::set_texture_config(TextureInformation& config, const tinygltf::Model& model, const tinygltf::Texture& texture, const tinygltf::Image& image)
	{
		// Sampler parameters (sampler could not exist)
		if (texture.sampler >= 0)
		{
			const tinygltf::Sampler& sampler = model.samplers[texture.sampler];
			config.m_minFilter = sampler.minFilter >= 0 ? sampler.minFilter : config.m_minFilter;
			config.m_magFilter = sampler.magFilter >= 0 ? sampler.magFilter : config.m_magFilter;
			config.m_sWrap = sampler.wrapS;
			config.m_tWrap = sampler.wrapT;
		}

		config.m_width = image.width;
		config.m_height = image.height;
		config.m_componentType = image.pixel_type;

		config.m_format = GL_RGBA;
		if (image.component == 1)
			config.m_format = GL_RED;
		else if (image.component == 2)
			config.m_format = GL_RG;
		else if (image.component == 3)
			config.m_format = GL_RGB;
	}
}