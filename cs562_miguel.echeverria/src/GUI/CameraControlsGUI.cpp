/**
* @file CameraControlsGUI.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Logic of the imgui window that shows all the configurable camera parameters.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "CameraControlsGUI.h"
#include "Composition/Scene.h"
#include "Cameras/EditorCamera.h"
#include "Cameras/SphericalCamera.h"


namespace cs460
{
	CameraControlsGUI::CameraControlsGUI()
	{
	}


	void CameraControlsGUI::update()
	{
        Scene& scene = Scene::get_instance();

        if (ImGui::Begin("CAMERA CONTROLS"))
        {
            EditorCamera* editorCam = dynamic_cast<EditorCamera*>(scene.get_active_camera());
            SphericalCamera* sphericalCam = dynamic_cast<SphericalCamera*>(scene.get_active_camera());

            if (editorCam)
            {
                ImGui::Text("W/S: Movement along view vector");
                ImGui::Text("A/D: Movement along side vector");
                ImGui::Text("Q/E: Movement along world up vector");
                ImGui::Text("Right Click + Drag: Tilt the camera");

                ImGui::Separator();

                ImGui::SliderFloat("Camera Speed", &editorCam->m_movementSpeed, 0.1f, 100.0f);
                ImGui::SliderFloat("Camera Pan Speed", &editorCam->m_keyboardTiltSpeed, 10.0f, 1000.0f);
                ImGui::SliderFloat("Camera Mouse Tilt Speed", &editorCam->m_mouseTiltSpeed, 50.0f, 400.0f);
            }
            else if (sphericalCam)
            {
                ImGui::Text("CONTROLLER: Controller required for both player");
                ImGui::Text("movement and camera controls");
                ImGui::Separator();
                ImGui::Text("BLEND_1D_DEMO: The type of camera used in");
                ImGui::Text("this demo is a spherical camera");
                ImGui::Separator();
                ImGui::Text("RIGHT STICK: Standard third person");
                ImGui::Text("right stick camera controls");
                ImGui::Separator();
                ImGui::Text("BLEND_2D_DEMO: The 2d blending demo actually uses");
                ImGui::Text("a fixedcamera, but the controller is still");
                ImGui::Text("needed for movement");
                ImGui::NewLine();

                float currRadius = sphericalCam->get_radius();
                ImGui::SliderFloat("Spherical Cam Radius (only for 1d blending demo)", &currRadius, 1.0f, 10.0f, "%.2f");
                sphericalCam->set_radius(currRadius);
            }
        }

        ImGui::End();
	}
}