/**
* @file RendererConfigGUI.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Logic of the imgui window that shows all the configurable properties of the renderer.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "RendererConfigGUI.h"
#include "Composition/Scene.h"
#include "Graphics/Systems/Renderer.h"
#include "Graphics/Rendering/Skybox.h"
#include "Resources/ResourceManager.h"


namespace cs460
{
	RendererConfigGUI::RendererConfigGUI()
	{
	}


	void RendererConfigGUI::update()
	{
        Scene& scene = Scene::get_instance();
        Renderer& renderer = Renderer::get_instance();

        if (ImGui::Begin("RENDERER CONFIG"))
        {
            ImGui::Text("Light properties:");
            ImGui::InputFloat3("Direction", glm::value_ptr(scene.m_lightProperties.m_direction), "%.2f");

            ImGuiColorEditFlags flags = ImGuiColorEditFlags_::ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_::ImGuiColorEditFlags_NoAlpha;
            ImGui::ColorEdit3("Ambient", glm::value_ptr(scene.m_lightProperties.m_ambient), flags);
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(scene.m_lightProperties.m_diffuse), flags);
            ImGui::ColorEdit3("Specular", glm::value_ptr(scene.m_lightProperties.m_specular), flags);

            ImGui::Separator();

            bool useSkybox = renderer.get_skybox()->get_active();
            ImGui::Checkbox("Use Skybox", &useSkybox);
            renderer.get_skybox()->set_active(useSkybox);

            const std::string& skyboxDir = renderer.get_skybox()->get_directory();
            std::string previewName = skyboxDir;
            fs::path dirPath(previewName);
            previewName = dirPath.filename().generic_string();

            if (ImGui::BeginCombo("Skybox", previewName.c_str()))
            {
                try
                {
                    fs::path startingPath("data/CubeMaps/Skyboxes");

                    if (!fs::exists(startingPath) || !fs::is_directory(startingPath))
                    {
                        std::cout << "ERROR: Starting path doesn't exist or is not a directory\n";
                        ImGui::EndCombo();
                        return;
                    }

                    // For every directory, inside the skyboxes folder
                    for (const auto& dir_it : fs::directory_iterator(startingPath))
                    {
                        if (dir_it.is_directory())
                        {
                            const std::string& folderName = dir_it.path().filename().generic_string();
                            if (ImGui::Selectable(folderName.c_str()))
                            {
                                bool found = renderer.change_skybox(folderName);

                                // If the skybox is not already loaded, load it again
                                if (!found)
                                {
                                    ResourceManager::get_instance().load_skybox(folderName, dir_it.path().generic_string());
                                    renderer.change_skybox(folderName);
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

        ImGui::End();
	}
}