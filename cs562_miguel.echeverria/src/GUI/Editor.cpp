/**
* @file Editor.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Implementation of the Editor singleton class that is in charge of the main editor.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Editor.h"
#include "Graphics/Systems/Renderer.h"
#include "Composition/Scene.h"
#include "Composition/SceneNode.h"
#include "Graphics/Rendering/Skybox.h"
#include "Resources/ResourceManager.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>                 // For input (remove once own input system is done)



namespace cs460
{
    Editor::Editor()
    {
    }
    
    Editor::~Editor()
    {
    }
    
    Editor& Editor::get_instance()
    {
    	static Editor instance;
    	return instance;
    }
    

    // Setup imgui for rendering
    bool Editor::initialize()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
    
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(Renderer::get_instance().get_window().get_handle(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
    
    	return true;
    }


    // Start a new imgui frame and do all the gui logic
    void Editor::update()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuizmo::BeginFrame();

        Renderer& renderer = Renderer::get_instance();

        // TODO: Make own class for this
        if (glfwGetKey(renderer.get_window().get_handle(), GLFW_KEY_1) == GLFW_PRESS)
            m_state.m_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        else if (glfwGetKey(renderer.get_window().get_handle(), GLFW_KEY_2) == GLFW_PRESS)
            m_state.m_gizmoOperation = ImGuizmo::OPERATION::ROTATE;
        else if (glfwGetKey(renderer.get_window().get_handle(), GLFW_KEY_3) == GLFW_PRESS)
            m_state.m_gizmoOperation = ImGuizmo::OPERATION::SCALE;

        ImGuizmo::AllowAxisFlip(false);

        m_componentEditor.update();
        m_mainMenuBar.update();
        m_sceneGraphGui.update();

        Scene& scene = Scene::get_instance();

        // TODO: Make own class for this in the future
        // (CAMERA CONTROLS AND PARAMETERS)
        if (ImGui::Begin("CAMERA CONTROLS"))
        {
            ImGui::Text("W/S: Movement along view vector");
            ImGui::Text("A/D: Movement along side vector");
            ImGui::Text("Q/E: Movement along world up vector");
            ImGui::Text("Right Click + Drag: Tilt the camera");

            ImGui::Separator();

            ImGui::SliderFloat("Camera Speed", &scene.get_camera().m_movementSpeed, 0.1f, 100.0f);
            ImGui::SliderFloat("Camera Pan Speed", &scene.get_camera().m_keyboardTiltSpeed, 10.0f, 1000.0f);
            ImGui::SliderFloat("Camera Mouse Tilt Speed", &scene.get_camera().m_mouseTiltSpeed, 50.0f, 400.0f);
        }

        ImGui::End();


        // TODO: Make own class for this in the future
        // (LIGHT PROPERTIES)
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


        // Gizmo's drawing
        const float* viewMtx = glm::value_ptr(scene.get_camera().get_view_mtx());
        const float* projMtx = glm::value_ptr(scene.get_camera().get_projection_mtx());
        glm::mat4 modelMtx;

        if (m_state.m_selectedNode)
        {
            modelMtx = m_state.m_selectedNode->m_worldTr.get_model_mtx();

            // Set perspective projection
            ImGuizmo::SetOrthographic(false);
            //ImGuizmo::SetDrawlist();

            ImGuiIO& io = ImGui::GetIO();
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
            ImGuizmo::Manipulate(viewMtx, projMtx, m_state.m_gizmoOperation, m_state.m_gizmoMode, glm::value_ptr(modelMtx));
            //ImGuizmo::DrawGrid(viewMtx, projMtx, glm::value_ptr(modelMtx), 10.0f);

            // Update the position/orientation/scale only when modified
            if (ImGuizmo::IsUsing())
            {
                // Take the world position, orientation and scale from the matrix passed to manipulate
                glm::vec3 childWorldPos = modelMtx[3];
                glm::vec3 childWorldScale = glm::vec3(glm::length(modelMtx[0]), glm::length(modelMtx[1]), glm::length(modelMtx[2]));
                glm::mat3 orientationMtx;
                orientationMtx[0] = glm::normalize(modelMtx[0]);
                orientationMtx[1] = glm::normalize(modelMtx[1]);
                orientationMtx[2] = glm::normalize(modelMtx[2]);
                glm::quat childWorldOrientation = glm::quat_cast(orientationMtx);

                SceneNode* parent = m_state.m_selectedNode->get_parent();
                // If there is no parent, the world transform is the local transform
                if (parent == nullptr)
                {
                    m_state.m_selectedNode->m_localTr.m_position = childWorldPos;
                    m_state.m_selectedNode->m_localTr.m_scale = childWorldScale;
                    m_state.m_selectedNode->m_localTr.m_orientation = childWorldOrientation;
                }
                // Otherwise, do inverse concatenation to obtain the local transform of the selected object from its world transform
                else
                {
                    float invScaleX = parent->m_worldTr.m_scale.x > FLT_EPSILON ? (1.0f / parent->m_worldTr.m_scale.x) : 0.0f;
                    float invScaleY = parent->m_worldTr.m_scale.y > FLT_EPSILON ? (1.0f / parent->m_worldTr.m_scale.y) : 0.0f;
                    float invScaleZ = parent->m_worldTr.m_scale.z > FLT_EPSILON ? (1.0f / parent->m_worldTr.m_scale.z) : 0.0f;
                    glm::vec3 invScale = glm::vec3(invScaleX, invScaleY, invScaleZ);

                    glm::quat invRotation = glm::inverse(parent->m_worldTr.m_orientation);

                    m_state.m_selectedNode->m_localTr.m_scale = invScale * childWorldScale;
                    m_state.m_selectedNode->m_localTr.m_orientation = invRotation * childWorldOrientation;
                    m_state.m_selectedNode->m_localTr.m_position = invScale * (invRotation * (childWorldPos - parent->m_worldTr.m_position));
                }
            }
        }

        // Show the demo window for reference
        ImGui::ShowDemoWindow();
    }


    // Render the gui
    void Editor::render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    // Terminate imgui
    void Editor::close()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    EditorState& Editor::get_state()
    {
        return m_state;
    }
}