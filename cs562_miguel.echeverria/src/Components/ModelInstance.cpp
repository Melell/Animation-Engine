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
#include "Composition/Scene.h"
#include <imgui/imgui.h>
#include <gltf/tiny_gltf.h>


namespace cs460
{
	ModelInstance::ModelInstance()
	{
		Scene& scene = Scene::get_instance();
		auto& allModelNodes = scene.get_all_model_nodes();
		m_instanceId = (unsigned)allModelNodes.size();
		allModelNodes.push_back({});
	}

	ModelInstance::~ModelInstance()
	{
		//std::cout << "MODEL INSTANCE DESTRUCTOR\n";
	}

	// Create all the nodes from the given Model resource, with their corresponding components
	void ModelInstance::generate_nodes(Model* model)
	{
		if (model == nullptr)
			return;

		m_model = model;

		Scene& scene = Scene::get_instance();
		auto& instanceNodes = scene.get_model_inst_nodes(m_instanceId);
		instanceNodes.clear();

		GLTFScene& gltfScene = model->m_scenes[model->m_defaultScene];
		std::vector<int>& nodesIndices = gltfScene.m_nodeIndices;

		// Create the immediate children, which will create their own childrens
		for (int i = 0; i < nodesIndices.size(); ++i)
		{
			SceneNode* child = get_owner()->create_child(model->m_nodes[i].m_name);
			child->from_node_resource(model, i, this);
		}
	}

	unsigned ModelInstance::get_instance_id() const
	{
		return m_instanceId;
	}


	void ModelInstance::on_gui()
	{
		if (ImGui::BeginCombo("GLTF File", m_previewName.c_str()))
		{
			try
			{
				fs::path startingPath("data/Models");

				if (!fs::exists(startingPath) || !fs::is_directory(startingPath))
				{
					std::cout << "ERROR: Starting path doesn't exist or is not a directory\n";
					ImGui::EndCombo();
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
							Scene& scene = Scene::get_instance();
							get_owner()->delete_all_children();

							// Get the model from the resource manager (load if it is not already there), and generate the nodes
							Model* model = ResourceManager::get_instance().get_model(dir_it.path().generic_string());
							generate_nodes(model);
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