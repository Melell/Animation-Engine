/**
* @file Gizmos.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Logic of the editor gizmos, using ImGuizmo library.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Gizmos.h"
#include "Composition/Scene.h"
#include "EditorState.h"
#include "Composition/SceneNode.h"
#include "Platform/InputMgr.h"
#include "Cameras/ICamera.h"


namespace cs460
{
	Gizmos::Gizmos()
	{
	}


	void Gizmos::update()
	{
        ImGuiIO& io = ImGui::GetIO();
        Scene& scene = Scene::get_instance();
        EditorState& state = EditorState::get_main_editor_state();
        InputMgr& inputMgr = InputMgr::get_instance();


        // Only allow to change gizmo mode if imgui has not keyboard focus
        if (!io.WantCaptureKeyboard)
        {
            if (inputMgr.is_key_pressed(KEYS::key_1))
            {
                state.m_gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                m_noOperation = false;
            }
            else if (inputMgr.is_key_pressed(KEYS::key_2))
            {
                state.m_gizmoOperation = ImGuizmo::OPERATION::ROTATE;
                m_noOperation = false;
            }
            else if (inputMgr.is_key_pressed(KEYS::key_3))
            {
                state.m_gizmoOperation = ImGuizmo::OPERATION::SCALE;
                m_noOperation = false;
            }
            else if (inputMgr.is_key_pressed(KEYS::key_4))
                m_noOperation = true;
        }


        // Don't do the gizmos logic if the no operation mode is selected
        if (m_noOperation)
            return;

        ImGuizmo::AllowAxisFlip(false);


        // Gizmo's drawing
        const float* viewMtx = glm::value_ptr(scene.get_active_camera()->get_view_mtx());
        const float* projMtx = glm::value_ptr(scene.get_active_camera()->get_projection_mtx());
        glm::mat4 modelMtx;

        if (state.m_selectedNode)
        {
            modelMtx = state.m_selectedNode->m_worldTr.get_model_mtx();

            // Set perspective projection
            ImGuizmo::SetOrthographic(false);
            //ImGuizmo::SetDrawlist();

            ImGuiIO& io = ImGui::GetIO();
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
            ImGuizmo::Manipulate(viewMtx, projMtx, state.m_gizmoOperation, state.m_gizmoMode, glm::value_ptr(modelMtx));
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

                SceneNode* parent = state.m_selectedNode->get_parent();
                // If there is no parent, the world transform is the local transform
                if (parent == nullptr)
                {
                    state.m_selectedNode->m_localTr.m_position = childWorldPos;
                    state.m_selectedNode->m_localTr.m_scale = childWorldScale;
                    state.m_selectedNode->m_localTr.m_orientation = childWorldOrientation;
                }
                // Otherwise, do inverse concatenation to obtain the local transform of the selected object from its world transform
                else
                {
                    TransformData childWorld;
                    childWorld.m_position = childWorldPos;
                    childWorld.m_orientation = childWorldOrientation;
                    childWorld.m_scale = childWorldScale;

                    state.m_selectedNode->m_localTr.inverse_concatenate(childWorld, parent->m_worldTr);
                }
            }
        }
	}
}