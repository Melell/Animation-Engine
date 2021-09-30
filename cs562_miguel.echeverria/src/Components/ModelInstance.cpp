/**
* @file Model.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Model component that can load a gltf file.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ModelInstance.h"
#include "IComponent.h"
#include "Graphics/Model.h"
#include "Resources/ResourceManager.h"
#include "Composition/SceneNode.h"
#include <imgui/imgui.h>
#include <gltf/tiny_gltf.h>


namespace cs460
{
	// Load all the nodes from the gltf filePath onto the scene graph, each referencing their corresponding mesh
	void ModelInstance::load_gltf_nodes(const std::string& filePath)
	{
		// Use tinygltf to parse the file and get the data
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string errorStr;
		std::string warningStr;

		bool ret = loader.LoadASCIIFromFile(&model, &errorStr, &warningStr, filePath);
		//bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

		if (!warningStr.empty())
			std::cout << "TINYGLTF WARNING: " << warningStr << std::endl;

		if (!errorStr.empty())
			std::cout << "TINYGLTF ERROR: " << errorStr << std::endl;


		if (!ret) {
			std::cout << "TINYGLTF: Failed to parse glTF\n";
			return;
		}

		process_nodes_data(model);
	}

	// Process the model structure to create the necessary nodes in the scene graph
	void ModelInstance::process_nodes_data(const tinygltf::Model& model)
	{
		int sceneIdx = model.defaultScene > 0 ? model.defaultScene : 0;
		const std::vector<int>& nodesIndices = model.scenes[sceneIdx].nodes;

		// Create the immeadiate children, and generate their children and other data
		for (int i = 0; i < nodesIndices.size(); ++i)
		{
			const tinygltf::Node& node = model.nodes[nodesIndices[i]];
			SceneNode* child = get_owner()->create_child(node.name);
			child->from_gltf_node(model, node, m_model, get_owner());
		}
	}


	void ModelInstance::on_gui()
	{
		if (ImGui::BeginCombo("GLTF File", m_previewName.c_str()))
		{
			try
			{
				fs::path startingPath("data");

				if (!fs::exists(startingPath) || !fs::is_directory(startingPath))
				{
					std::cout << "ERROR: Starting path doesn't exist or is not a directory\n";
					return;
				}

				// For every .gltf file in the subdirectories
				for (const auto& dir_it : fs::recursive_directory_iterator(startingPath))
				{
					if (dir_it.is_regular_file() && dir_it.path().extension().generic_string() == ".gltf")
					{
						const std::string& filename = dir_it.path().filename().generic_string();
						if (ImGui::Selectable(filename.c_str()))
						{
							m_previewName = filename;
							m_model = ResourceManager::get_instance().get_model(dir_it.path().generic_string());
							load_gltf_nodes(dir_it.path().generic_string());
						}
					}
				}
			}
			catch (const fs::filesystem_error& error)
			{
				std::cout << "FILESYSTEM ERROR: " << error.what() << std::endl;
			}

			ImGui::EndCombo();
		}
	}
}