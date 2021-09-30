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
#include <gltf/tiny_gltf.h>
#include <GL/glew.h>


namespace cs460
{
	Primitive::Primitive()
	{

	}


	Primitive::~Primitive()
	{
		delete_gl_buffers();
	}


	// Process the primitive's data from a tinygltf model's primitive into this one
	void Primitive::process_primitive_data(const tinygltf::Model& model, const tinygltf::Primitive& primitive)
	{
		// Store the necessary variables for drawing later on
		m_mode = primitive.mode == -1 ? GL_TRIANGLES : primitive.mode;
		if (primitive.indices >= 0)
		{
			const tinygltf::Accessor& idxAccessor = model.accessors[primitive.indices];
			m_offset = idxAccessor.byteOffset;
			m_elementCount = idxAccessor.count;
			m_eboComponentType = idxAccessor.componentType;
			m_usesEbo = true;
		}
		else
		{
			m_elementCount = model.bufferViews[0].byteLength;
			m_usesEbo = false;
		}

		// Generate and bind the vertex array object
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// Create a vbo for each buffer view, and upload its corresponding data
		m_vbos.resize(model.bufferViews.size());
		for (int i = 0; i < model.bufferViews.size(); ++i)
		{
			const tinygltf::BufferView& bufView = model.bufferViews[i];

			// If target is 0 (neither array or elements, ignore this buffer)
			if (bufView.target == 0)
				continue;

			glGenBuffers(1, &m_vbos[i]);
			glBindBuffer(bufView.target, m_vbos[i]);

			const std::vector<unsigned char>& buffer = model.buffers[bufView.buffer].data;
			glBufferData(bufView.target, bufView.byteLength, buffer.data() + bufView.byteOffset, GL_STATIC_DRAW);
		}

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

			// Select the location for the current attribute
			int attArrayIdx = 0;
			if (attName == "POSITION")
				attArrayIdx = 0;
			else if (attName == "NORMAL")
				attArrayIdx = 1;
			else if (attName == "TEXCOORD_0")
				attArrayIdx = 2;

			// Get the number of components in each vertex attribute
			int attComponentCount = 1;
			if (attAccessor.type != TINYGLTF_TYPE_SCALAR)
				attComponentCount = attAccessor.type;

			// Just to make sure if they are the same
			assert(attAccessor.count == attComponentCount);

			// Get the stride of the attribute, and whether it is normalized
			int stride = attAccessor.ByteStride(bufView);
			GLboolean normalized = attAccessor.normalized ? GL_TRUE : GL_FALSE;

			// Bind the vbo this attribute refers to, and tell OpenGL how to interpret the attribute data within the vbo
			glBindBuffer(bufView.target, attAccessor.bufferView);
			glEnableVertexAttribArray(attArrayIdx);
			glVertexAttribPointer(attArrayIdx, attComponentCount, attAccessor.componentType, normalized, stride, (void*)attAccessor.byteOffset);
		}

		glBindVertexArray(0);
	}


	// Draw the primitive
	void Primitive::render() const
	{
		glBindVertexArray(m_vao);

		if (m_usesEbo)
			glDrawElements(m_mode, m_elementCount, m_eboComponentType, (void*)m_offset);
		else
			glDrawArrays(m_mode, 0, m_elementCount);

		glBindVertexArray(0);
	}


	// Free all the opengl buffers used by this primitive
	void Primitive::delete_gl_buffers()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers((int)m_vbos.size(), m_vbos.data());
	}
}