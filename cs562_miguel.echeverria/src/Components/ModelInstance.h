/**
* @file Model.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Model component that can load a gltf file.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"

namespace tinygltf
{
	class Model;
}


namespace cs460
{
	class Model;


	class ModelInstance : public IComponent
	{
	public:

		virtual ~ModelInstance();

		// Load all the nodes from the gltf filePath onto the scene graph, each referencing their corresponding mesh
		void load_gltf_nodes(const std::string& filePath);

	private:

		Model* m_model = nullptr;
		std::string m_previewName = "Empty";

		// Process the model structure to create the necessary nodes in the scene graph
		void process_nodes_data(const tinygltf::Model& model);
		void on_gui() override;
	};
}