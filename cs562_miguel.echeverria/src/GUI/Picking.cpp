/**
* @file Picking.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief In charge of all the picking logic.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Picking.h"
#include "Platform/InputMgr.h"
#include "Graphics/Systems/Renderer.h"
#include "Composition/Scene.h"
#include "Math/Geometry/Geometry.h"
#include "Components/Models/MeshRenderable.h"
#include "EditorState.h"
#include "Animation/PiecewiseCurveMgr.h"
#include "Cameras/ICamera.h"


namespace cs460
{
	Picking::Picking()
	{
	}


	void Picking::update()
	{
		InputMgr& inputMgr = InputMgr::get_instance();

		// If user left-clicked
		if (inputMgr.is_mouse_button_pressed(MOUSE::button_left))
		{
			Renderer& renderer = Renderer::get_instance();
			Scene& scene = Scene::get_instance();
			ICamera* cam = scene.get_active_camera();
			PiecewiseCurveMgr& curveMgr = PiecewiseCurveMgr::get_instance();


			// Get a normalized ray in world space from the camera position with the direction given by the mouse click
			const glm::vec3& worldRayDir = viewport_to_world_normalized_ray(inputMgr.get_mouse_position());


			// Check with this ray against the bv of all meshes
			Ray worldRay;
			worldRay.m_origin = cam->get_position();
			worldRay.m_dir = worldRayDir;
			float meshTime = 0.0f;
			MeshRenderable* pickedMesh = renderer.ray_vs_meshes(worldRay, &meshTime);


			// Check with this ray against the bv of all curve elements (points/tangents/control points)
			float curveElementTime = 0.0f;
			IComponent* pickedCurveElement = curveMgr.ray_vs_curve_elements(worldRay, &curveElementTime);


			EditorState& editorState = EditorState::get_main_editor_state();


			// Only update if not using gizmos, or not hovering an imgui window
			if (!ImGuizmo::IsUsing() && !ImGui::GetIO().WantCaptureMouse)
			{
				// Update the currently selected object (min t of intersection between possibly selected mesh and curve element)
				if (pickedMesh || pickedCurveElement)
				{
					if (pickedMesh && pickedCurveElement)
						editorState.m_selectedNode = meshTime <= curveElementTime ? pickedMesh->get_owner() : pickedCurveElement->get_owner();
					else if (pickedMesh)
						editorState.m_selectedNode = pickedMesh->get_owner();
					else
						editorState.m_selectedNode = pickedCurveElement->get_owner();
				}
				else
					editorState.m_selectedNode = nullptr;
			}
		}
	}



	glm::vec3 Picking::viewport_to_world_normalized_ray(const glm::vec2& viewportCoords)
	{
		const glm::vec3& ndcPoint = viewport_to_ndc(viewportCoords);
		const glm::vec4& clipSpacePoint = ndc_to_homogeneuos_clip_space(ndcPoint);
		const glm::vec4& camSpaceRay = homogeneuos_clip_space_to_camera_space(clipSpacePoint);
		const glm::vec3& worldSpaceNormalizedRay = camera_to_world_space(camSpaceRay);
		return worldSpaceNormalizedRay;
	}


	glm::vec3 Picking::viewport_to_ndc(const glm::vec2& viewportCoords)
	{
		Renderer& renderer = Renderer::get_instance();
		Window& window = renderer.get_window();

		// We are assuming the size of the framebuffer is the same as the viewport(only one camera)
		int fbWidth, fbHeight;
		window.get_framebuffer_dimensions(&fbWidth, &fbHeight);

		// Get the normalized coordinates
		float x = viewportCoords.x / fbWidth;
		float y = viewportCoords.y / fbHeight;
		float z = 1.0f;

		// Transform them to the range [-1, 1], and invert axis (for y-coordinate)
		x = 2 * x - 1;
		y = 1 - 2 * y;

		return glm::vec3(x, y, z);
	}

	glm::vec4 Picking::ndc_to_homogeneuos_clip_space(const glm::vec3& ndcCoords)
	{
		return glm::vec4(ndcCoords.x, ndcCoords.y, -1.0f, 1.0f);
	}

	glm::vec4 Picking::homogeneuos_clip_space_to_camera_space(const glm::vec4& clipCoords)
	{
		Scene& scene = Scene::get_instance();
		ICamera* cam = scene.get_active_camera();

		const glm::mat4& invProjectionMtx = glm::inverse(cam->get_projection_mtx());
		const glm::vec4& cameraPoint = invProjectionMtx * clipCoords;

		return glm::vec4(cameraPoint.x, cameraPoint.y, -1.0f, 0.0f);
	}

	glm::vec3 Picking::camera_to_world_space(const glm::vec4& camCoords)
	{
		Scene& scene = Scene::get_instance();
		ICamera* cam = scene.get_active_camera();

		const glm::mat4& invViewMtx = glm::inverse(cam->get_view_mtx());
		const glm::vec3& worldRay = glm::vec3(invViewMtx * camCoords);
		return glm::normalize(worldRay);
	}
}