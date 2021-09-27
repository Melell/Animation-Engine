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
#include "Graphics/Renderer.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>



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

        m_mainMenuBar.update();
        m_sceneGraphGui.update();
        m_componentEditor.update();

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