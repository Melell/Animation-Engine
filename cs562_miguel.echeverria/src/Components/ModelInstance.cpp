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
#include "Graphics/GLTF/Model.h"
#include "Resources/ResourceManager.h"
#include "Composition/SceneNode.h"
#include "Composition/Scene.h"
#include "Components/AnimationReference.h"
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


	// Create all the nodes/components from the internal Model resource
	void ModelInstance::generate_nodes()
	{
		if (m_model == nullptr)
			return;

		// Reset the dictionary of nodes
		Scene& scene = Scene::get_instance();
		auto& instanceNodes = scene.get_model_inst_nodes(m_instanceId);
		instanceNodes.clear();


		// Create the immediate children, which will create their own childrens
		GLTFScene& gltfScene = m_model->m_scenes[m_model->m_defaultScene];
		std::vector<int>& nodesIndices = gltfScene.m_nodeIndices;

		for (int i = 0; i < nodesIndices.size(); ++i)
		{
			int childIdx = nodesIndices[i];
			SceneNode* child = get_owner()->create_child(m_model->m_nodes[childIdx].m_name);
			child->from_node_resource(m_model, childIdx, this);
		}
	}

	void ModelInstance::generate_components()
	{
		if (m_model == nullptr)
			return;

		Scene& scene = Scene::get_instance();
		auto& modelNodes = scene.get_model_inst_nodes(m_instanceId);

		add_anim_comp();		

		// Create the immediate children's components, which will create their own childrens' components
		GLTFScene& gltfScene = m_model->m_scenes[m_model->m_defaultScene];
		std::vector<int>& nodesIndices = gltfScene.m_nodeIndices;

		for (int i = 0; i < nodesIndices.size(); ++i)
		{
			int childIdx = nodesIndices[i];
			SceneNode* childNode = modelNodes[childIdx];
			childNode->generate_components(childIdx);
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
							if (model != nullptr)
							{
								get_owner()->set_model_source(m_model);
								get_owner()->set_model_root_node(get_owner());

								m_model = model;
								generate_nodes();
								generate_components();
							}
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


	void ModelInstance::add_anim_comp()
	{
		// Add the animation reference component to the node with the model instance component
		AnimationReference* anim = get_owner()->add_component<AnimationReference>();
	}
}