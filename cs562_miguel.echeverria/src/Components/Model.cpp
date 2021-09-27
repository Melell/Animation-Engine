/**
* @file Model.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Model component that can load a gltf file.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Model.h"
#include "IComponent.h"
#include <imgui/imgui.h>
#include <gltf/tiny_gltf.h>


namespace cs460
{
	void Model::load_gltf()
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string errorStr;
		std::string warningStr;

		bool ret = loader.LoadASCIIFromFile(&model, &errorStr, &warningStr, m_filePath);
		//bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

		if (!warningStr.empty())
			std::cout << "TINYGLTF WARNING: " << warningStr << std::endl;

		if (!errorStr.empty())
			std::cout << "TINYGLTF ERROR: " << errorStr << std::endl;


		if (!ret) {
			std::cout << "TINYGLTF: Failed to parse glTF\n";
			return;
		}

		process_model_data(model);
	}

	void Model::process_model_data(const tinygltf::Model& model)
	{
		model.scenes[0].name;	// Name of the root?

		for (int nodeIdx : model.scenes[0].nodes)
		{
			model.nodes[nodeIdx];
		}
	}

	void Model::on_gui()
	{
		if (ImGui::BeginCombo("GLTF File", m_fileName.c_str()))
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
							m_fileName = filename;
							m_filePath = dir_it.path().generic_string();
							load_gltf();
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