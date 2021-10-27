/**
* @file MainMenuBarGUI.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief In charge of the GUI for the main menu toolbar at the top of the viewport.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "MainMenuBarGUI.h"
#include <imgui/imgui.h>



namespace cs460
{
	MainMenuBarGUI::MainMenuBarGUI()
	{
	}


	void MainMenuBarGUI::update()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Load Scene"))
			{
				//try
				//{
				//	fs::path startingPath("data");
				//
				//	if (!fs::exists(startingPath) || !fs::is_directory(startingPath))
				//	{
				//		std::cout << "ERROR: Starting path doesn't exist or is not a directory\n";
				//		return;
				//	}
				//
				//	// For every .gltf file in the subdirectories
				//	for (const auto& dir_it : fs::recursive_directory_iterator(startingPath))
				//		if (dir_it.is_regular_file() && dir_it.path().extension().generic_string() == ".gltf")
				//			ImGui::MenuItem(dir_it.path().filename().generic_string().c_str());
				//}
				//catch (const fs::filesystem_error& error)
				//{
				//	std::cout<<"FILESYSTEM ERROR: "<<error.what()<<std::endl;
				//}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug Drawing"))
			{
				if (ImGui::BeginMenu("Bounding Volumes"))
				{
					static int bvRenderMode = 0;
					ImGui::RadioButton("None", &bvRenderMode, 0);
					ImGui::RadioButton("Selected", &bvRenderMode, 1);
					ImGui::RadioButton("All", &bvRenderMode, 2);

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}