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
#include "Graphics/Systems/Renderer.h"
#include "Composition/Scene.h"								// For loading scenes manually (remove in the future)
#include "Editor.h"											// TODO: Remove this in the future
#include "Composition/SceneNode.h"							// For loading scenes manually (remove in the future)
#include "Components/PiecewiseCurves/PiecewiseCurve.h"		// Remove in future
#include "Components/PiecewiseCurves/CurvePoint.h"			// Remove in future
#include "Components/PiecewiseCurves/CurveTangent.h"		// Remove in future
#include "Components/PiecewiseCurves/CurveControlPoint.h"	// Remove in future
#include "Components/Models/ModelInstance.h"				// Remove in future
#include "Components/Animation/AnimationReference.h"		// Remove in future
#include "Graphics/Systems/DebugRenderer.h"
#include "Animation/Blending/Blend1D.h"
#include "Animation/Blending/Blend2D.h"
#include "Animation/Blending/BlendAnim.h"
#include "Resources/ResourceManager.h"
#include "Graphics/GLTF/Model.h"
#include "Gameplay/Components/PlayerController.h"
#include "Cameras/EditorCamera.h"
#include "Cameras/SphericalCamera.h"
#include "Animation/InverseKinematics/IKChain.h"
#include "Animation/InverseKinematics/Analytic2Bone2DSolver.h"
#include "Components/Animation/IKChainRoot.h"
#include "Graphics/Rendering/Skybox.h"
#include "Components/Particles/Cloth.h"



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
				if (ImGui::MenuItem("Empty"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::EMPTY;
				}

				if (ImGui::MenuItem("Linear Curve"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::LINEAR;
				}
				if (ImGui::MenuItem("Hermite Curve"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::HERMITE;
				}
				if (ImGui::MenuItem("Catmull-Rom Curve"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::CATMULL_ROM;
				}
				if (ImGui::MenuItem("Bezier Curve"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::BEZIER;
				}
				if (ImGui::MenuItem("Skinned Animation"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::SKINNED_ANIMATION;
				}
				if (ImGui::MenuItem("NESTED MODELS"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::NESTED_MODELS;
				}
				if (ImGui::MenuItem("PATH FOLLOWING"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::PATH_FOLLOWING;
				}
				if (ImGui::MenuItem("Blending 1D"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::BLENDING_1D;
				}
				if (ImGui::MenuItem("Blending 2D"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::BLENDING_2D;
				}
				if (ImGui::MenuItem("1D blend editor"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::BLEND_EDITOR_1D;
				}
				if (ImGui::MenuItem("2D blend editor"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::BLEND_EDITOR_2D;
				}
				if (ImGui::MenuItem("IK Analytic 2D"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::IK_ANALYTICAL_2D;
				}
				if (ImGui::MenuItem("IK Ccd 3D"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::IK_CCD_3D;
				}
				if (ImGui::MenuItem("IK Fabrik 3D"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::IK_FABRIK_3D;
				}
				if (ImGui::MenuItem("IK On Skeleton"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::IK_ON_SKELETON;
				}
				if (ImGui::MenuItem("Cloth Simulation"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::CLOTH_SIMULATION;
				}
				if (ImGui::MenuItem("Cloth Collision"))
				{
					m_sceneToLoad = SCENE_TO_LOAD::CLOTH_COLLISION;
				}
				

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug Drawing"))
			{
				Renderer& renderer = Renderer::get_instance();

				if (ImGui::BeginMenu("Bounding Volumes"))
				{
					ImGui::Checkbox("Enable Drawing", &renderer.m_drawAnyBv);

					//ImGui::ColorButton("Color Button", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					//static glm::vec4 color;
					//ImGui::ColorPicker4("Color Picker", glm::value_ptr(color));
					ImGui::ColorEdit4("BVs Color", glm::value_ptr(DebugRenderer::s_bvsColor));

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Skeletons"))
				{
					ImGui::Checkbox("Enable Drawing", &DebugRenderer::s_enableSkeletonDrawing);
					ImGui::ColorEdit4("Bone Color", glm::value_ptr(DebugRenderer::s_boneColor));
					ImGui::ColorEdit4("Joint Color", glm::value_ptr(DebugRenderer::s_jointColor));
					ImGui::DragFloat("Joint Size", &DebugRenderer::s_jointSize, 0.005f, 0.01f, 2.0f, "%.3f");
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Curves"))
				{
					ImGui::Text("Moving Object");
					ImGui::Checkbox("Enable Drawing##0", &DebugRenderer::s_enableMovingObjectDrawing);
					ImGui::ColorEdit4("Moving Object Color", glm::value_ptr(DebugRenderer::s_movingObjectColor));
					ImGui::DragFloat("Moving Object Size", &DebugRenderer::s_movingObjectSize, 0.01f, 0.01f, 5.0f, "%.3f");

					ImGui::Separator();

					ImGui::Text("Curve / Curve Points");
					ImGui::Checkbox("Enable Drawing##1", &DebugRenderer::s_enableCurveDrawing);
					ImGui::ColorEdit4("Curve Color", glm::value_ptr(DebugRenderer::s_curveColor));
					ImGui::ColorEdit4("Curve Points Color", glm::value_ptr(DebugRenderer::s_curvePointColor));
					ImGui::DragFloat("Curve Points Size", &DebugRenderer::s_curvePointSize, 0.01f, 0.01f, 5.0f, "%.3f");

					ImGui::Separator();

					ImGui::Text("Curve Tangents / Control Points");
					ImGui::Checkbox("Enable Drawing##2", &DebugRenderer::s_enableTangentDrawing);
					ImGui::ColorEdit4("Lines Color", glm::value_ptr(DebugRenderer::s_tangentLineColor));
					ImGui::ColorEdit4("Endpoints Color", glm::value_ptr(DebugRenderer::s_tangentEndpointColor));
					ImGui::DragFloat("Endpoints Size", &DebugRenderer::s_tangentEndpointSize, 0.01f, 0.01f, 5.0f, "%.3f");

					ImGui::Separator();

					ImGui::Text("TABLE SAMPLES");
					ImGui::Checkbox("Enable Drawing##3", &DebugRenderer::s_enableTableSamplesDrawing);
					ImGui::ColorEdit4("Samples Color", glm::value_ptr(DebugRenderer::s_tableSampleColor));
					ImGui::DragFloat("Samples Size", &DebugRenderer::s_tableSampleSize, 0.01f, 0.01f, 5.0f, "%.3f");


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Grid"))
				{
					ImGui::Checkbox("Enable Drawing", &DebugRenderer::s_enableGridDrawing);
					ImGui::SliderFloat("Grid X Size", &DebugRenderer::s_xGridSize, 10.0f, 100.0f, "%.1f");
					ImGui::SliderFloat("Grid Z Size", &DebugRenderer::s_zGridSize, 10.0f, 100.0f, "%.1f");

					int xSubdivisions = (int)DebugRenderer::s_xSubdivisions;
					int zSubdivisions = (int)DebugRenderer::s_zSubdivisions;
					ImGui::SliderInt("X Subdivisions", &xSubdivisions, 0, 200);
					ImGui::SliderInt("Z Subdivisions", &zSubdivisions, 0, 200);
					xSubdivisions = glm::clamp(xSubdivisions, 0, 200);
					zSubdivisions = glm::clamp(zSubdivisions, 0, 200);
					DebugRenderer::s_xSubdivisions = (unsigned)xSubdivisions;
					DebugRenderer::s_zSubdivisions = (unsigned)zSubdivisions;

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("IK Chain"))
				{
					ImGui::Checkbox("Enable Drawing", &DebugRenderer::s_enableIKChainDrawing);
					ImGui::ColorEdit4("Bone Success Color", glm::value_ptr(DebugRenderer::s_ikBoneColorSuccess));
					ImGui::ColorEdit4("Bone Failure Color", glm::value_ptr(DebugRenderer::s_ikBoneColorFailure));
					ImGui::ColorEdit4("Bone Idle Color", glm::value_ptr(DebugRenderer::s_ikBoneColorIdle));
					ImGui::ColorEdit4("Bone Processing Color", glm::value_ptr(DebugRenderer::s_ikBoneColorProcessing));
					ImGui::ColorEdit4("Bone Highlight Color", glm::value_ptr(DebugRenderer::s_ikBoneHighlightColor));
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("CLOTH"))
				{
					ImGui::Text("Cloth Draw Mode");
					ImGui::RadioButton("Debug Drawing", &DebugRenderer::s_clothDrawingMode, 0);
					ImGui::SameLine();
					ImGui::RadioButton("Normal Mapped Texture", &DebugRenderer::s_clothDrawingMode, 1);
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}


	MainMenuBarGUI& MainMenuBarGUI::get_main_menu_bar_gui()
	{
		return Editor::get_instance().get_main_menu_bar();
	}


	void MainMenuBarGUI::load_scene()
	{
		if (m_sceneToLoad == SCENE_TO_LOAD::NONE)
			return;

		if (m_sceneToLoad == SCENE_TO_LOAD::EMPTY)
			load_empty_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::LINEAR)
			load_linear_curve_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::HERMITE)
			load_hermite_curve_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::CATMULL_ROM)
			load_catmull_rom_curve_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::BEZIER)
			load_bezier_curve_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::SKINNED_ANIMATION)
			load_skinned_animation_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::NESTED_MODELS)
			load_nested_models_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::PATH_FOLLOWING)
			load_path_following_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::BLENDING_1D)
			load_blending_1d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::BLENDING_2D)
			load_blending_2d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::BLEND_EDITOR_1D)
			load_blend_editor_1d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::BLEND_EDITOR_2D)
			load_blend_editor_2d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::IK_ANALYTICAL_2D)
			load_ik_analytical_2d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::IK_CCD_3D)
			load_ik_ccd_3d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::IK_FABRIK_3D)
			load_ik_fabrik_3d_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::IK_ON_SKELETON)
			load_ik_on_skeleton_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::CLOTH_SIMULATION)
			load_cloth_simulation_scene();
		else if (m_sceneToLoad == SCENE_TO_LOAD::CLOTH_COLLISION)
			load_cloth_collision_scene();

		m_sceneToLoad = SCENE_TO_LOAD::NONE;
	}


	void MainMenuBarGUI::load_empty_scene()
	{
		Scene& scene = Scene::get_instance();
		scene.clear();
		scene.m_lightProperties.m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);
		Editor::get_instance().get_state().m_selectedNode = nullptr;

		DebugRenderer::s_enableGridDrawing = false;
		DebugRenderer::s_enableSkeletonDrawing = false;
		scene.change_camera(true);
		scene.get_active_camera()->set_is_active(true);
		Renderer::get_instance().get_skybox()->set_active(true);
	}

	void MainMenuBarGUI::load_linear_curve_scene()
	{
		// Clear the scene
		load_empty_scene();

		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();
		
		// Create the curve node with a PiecewiseCurve component
		SceneNode* curveNode = root->create_child("LINEAR PIECEWISE CURVE");
		curveNode->add_component<PiecewiseCurve>()->initialize();
	
		// Create the curve point nodes
		SceneNode* curvePoint0 = curveNode->create_child("Curve Point 0");
		SceneNode* curvePoint1 = curveNode->create_child("Curve Point 1");
		SceneNode* curvePoint2 = curveNode->create_child("Curve Point 2");
		SceneNode* curvePoint3 = curveNode->create_child("Curve Point 3");
		SceneNode* curvePoint4 = curveNode->create_child("Curve Point 4");

		// Add to them the curve point components (and set their times)
		curvePoint0->add_component<CurvePoint>();
		curvePoint1->add_component<CurvePoint>()->set_time(1.0f);
		curvePoint2->add_component<CurvePoint>()->set_time(4.0f);
		curvePoint3->add_component<CurvePoint>()->set_time(5.0f);
		curvePoint4->add_component<CurvePoint>()->set_time(8.0f);

		// Set their positions
		curvePoint0->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		curvePoint1->m_localTr.m_position = glm::vec3(1.69f, 0.0f, 0.0f);
		curvePoint2->m_localTr.m_position = glm::vec3(-0.124f, -0.881f, 0.0f);
		curvePoint3->m_localTr.m_position = glm::vec3(1.705f, -0.898f, 0.0f);
		curvePoint4->m_localTr.m_position = glm::vec3(0.355f, -1.801f, 0.0f);

		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(0.8f, -0.8f, 3.0f));
			editorCam->set_target(editorCam->get_position() + glm::vec3(0.0f, 0.0f, -1.0f) * 25.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on linear curve demo\n";
	}

	void MainMenuBarGUI::load_hermite_curve_scene()
	{
		load_empty_scene();


		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		// Create the curve node with a PiecewiseCurve component
		SceneNode* curveNode = root->create_child("HERMITE PIECEWISE CURVE");
		PiecewiseCurve* curveComp = curveNode->add_component<PiecewiseCurve>();
		curveComp->initialize();
		curveComp->change_curve_type(CURVE_TYPE::HERMITE);
		curveComp->change_finish_mode(PiecewiseCurve::FINISH_MODE::PINPONG);

		// Create the curve point nodes
		SceneNode* curvePoint0 = curveNode->create_child("Curve Point 0");
		SceneNode* curvePoint1 = curveNode->create_child("Curve Point 1");
		SceneNode* curvePoint2 = curveNode->create_child("Curve Point 2");
		SceneNode* curvePoint3 = curveNode->create_child("Curve Point 3");

		// Add to them the curve point components (and set their times)
		curvePoint0->add_component<CurvePoint>();
		curvePoint1->add_component<CurvePoint>()->set_time(2.0f);
		curvePoint2->add_component<CurvePoint>()->set_time(4.0f);
		curvePoint3->add_component<CurvePoint>()->set_time(7.0f);


		// Add the tangents for point 0
		SceneNode* leftTangent0 = curvePoint0->create_child("Left Tangent 0");
		leftTangent0->add_component<CurveTangent>()->set_is_in_tangent(true);
		leftTangent0->m_localTr.m_position = glm::vec3(-0.916f, 0.0f, -0.818f);

		SceneNode* rightTangent0 = curvePoint0->create_child("Right Tangent 0");
		rightTangent0->add_component<CurveTangent>()->set_is_in_tangent(false);
		rightTangent0->m_localTr.m_position = glm::vec3(-0.644f, 1.991f, -1.932f);


		// Add the tangents for point 1
		SceneNode* leftTangent1 = curvePoint1->create_child("Left Tangent 1");
		leftTangent1->add_component<CurveTangent>()->set_is_in_tangent(true);
		leftTangent1->m_localTr.m_position = glm::vec3(-0.590f, -0.661f, 0.782f);

		SceneNode* rightTangent1 = curvePoint1->create_child("Right Tangent 1");
		rightTangent1->add_component<CurveTangent>()->set_is_in_tangent(false);
		rightTangent1->m_localTr.m_position = glm::vec3(0.845f, 1.224f, -0.917f);


		// Add the tangents for point 2
		SceneNode* leftTangent2 = curvePoint2->create_child("Left Tangent 2");
		leftTangent2->add_component<CurveTangent>()->set_is_in_tangent(true);
		leftTangent2->m_localTr.m_position = glm::vec3(0.258f, -2.156f, -1.447f);

		SceneNode* rightTangent2 = curvePoint2->create_child("Right Tangent 2");
		rightTangent2->add_component<CurveTangent>()->set_is_in_tangent(false);
		rightTangent2->m_localTr.m_position = glm::vec3(0.258f, -2.156f, -1.447f);


		// Add the tangents for point 3
		SceneNode* leftTangent3 = curvePoint3->create_child("Left Tangent 3");
		leftTangent3->add_component<CurveTangent>()->set_is_in_tangent(true);
		leftTangent3->m_localTr.m_position = glm::vec3(-0.355f, 8.891f, -0.313f);

		SceneNode* rightTangent3 = curvePoint3->create_child("Right Tangent 3");
		rightTangent3->add_component<CurveTangent>()->set_is_in_tangent(false);
		rightTangent3->m_localTr.m_position = glm::vec3(-0.594f, 0.0f, 0.266f);


		// Set the curve points' positions
		curvePoint0->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		curvePoint1->m_localTr.m_position = glm::vec3(2.801f, 0.0f, 0.0f);
		curvePoint2->m_localTr.m_position = glm::vec3(2.685f, 0.0f, 2.803f);
		curvePoint3->m_localTr.m_position = glm::vec3(-0.651f, 2.230f, 2.704f);



		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(-5.5f, 0.5f, 0.4f));
			editorCam->set_target(editorCam->get_position() + glm::vec3(0.8f, 0.0f, 0.1f) * 25.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on hermite curve demo\n";
	}

	void MainMenuBarGUI::load_catmull_rom_curve_scene()
	{
		load_empty_scene();


		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		// Create the curve node with a PiecewiseCurve component
		SceneNode* curveNode = root->create_child("CATMULL-ROM PIECEWISE CURVE");
		PiecewiseCurve* curveComp = curveNode->add_component<PiecewiseCurve>();
		curveComp->initialize();
		curveComp->change_curve_type(CURVE_TYPE::CATMULL_ROM);
		curveComp->change_finish_mode(PiecewiseCurve::FINISH_MODE::PINPONG);

		// Create the curve point nodes
		SceneNode* curvePoint0 = curveNode->create_child("Curve Point 0");
		SceneNode* curvePoint1 = curveNode->create_child("Curve Point 1");
		SceneNode* curvePoint2 = curveNode->create_child("Curve Point 2");
		SceneNode* curvePoint3 = curveNode->create_child("Curve Point 3");

		// Add to them the curve point components (and set their times)
		curvePoint0->add_component<CurvePoint>();
		curvePoint1->add_component<CurvePoint>()->set_time(2.0f);
		curvePoint2->add_component<CurvePoint>()->set_time(4.0f);
		curvePoint3->add_component<CurvePoint>()->set_time(6.0f);

		// Set their positions
		curvePoint0->m_localTr.m_position = glm::vec3(-1.041f, 1.624f, -2.537f);
		curvePoint1->m_localTr.m_position = glm::vec3(2.167f, -0.639f, -0.562f);
		curvePoint2->m_localTr.m_position = glm::vec3(2.141f, -0.289f, 1.609f);
		curvePoint3->m_localTr.m_position = glm::vec3(-2.011f, 0.472f, 0.558f);

		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(4.0f, 0.2f, 3.2f));
			editorCam->set_target(editorCam->get_position() + glm::vec3(-1.0f, -0.2f, -1.0f) * 25.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on catmull-rom curve demo\n";
	}

	void MainMenuBarGUI::load_bezier_curve_scene()
	{
		load_empty_scene();


		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		// Create the curve node with a PiecewiseCurve component
		SceneNode* curveNode = root->create_child("BEZIER PIECEWISE CURVE");
		PiecewiseCurve* curveComp = curveNode->add_component<PiecewiseCurve>();
		curveComp->initialize();
		curveComp->change_curve_type(CURVE_TYPE::BEZIER);
		curveComp->change_finish_mode(PiecewiseCurve::FINISH_MODE::RESTART);

		// Create the curve point nodes
		SceneNode* curvePoint0 = curveNode->create_child("Curve Point 0");
		SceneNode* curvePoint1 = curveNode->create_child("Curve Point 1");
		SceneNode* curvePoint2 = curveNode->create_child("Curve Point 2");

		// Add to them the curve point components (and set their times)
		curvePoint0->add_component<CurvePoint>();
		curvePoint1->add_component<CurvePoint>()->set_time(2.0f);
		curvePoint2->add_component<CurvePoint>()->set_time(4.0f);


		// Add the control points for point 0
		SceneNode* leftTangent0 = curvePoint0->create_child("Left Control Point 0");
		leftTangent0->add_component<CurveControlPoint>()->set_is_left_control_point(true);
		leftTangent0->m_localTr.m_position = glm::vec3(-0.666f, 0.0f, 0.504f);

		SceneNode* rightTangent0 = curvePoint0->create_child("Right Control Point 0");
		rightTangent0->add_component<CurveControlPoint>()->set_is_left_control_point(false);
		rightTangent0->m_localTr.m_position = glm::vec3(0.555f, 0.755f, -2.044f);


		// Add the control points for point 1
		SceneNode* leftTangent1 = curvePoint1->create_child("Left Control Point 1");
		leftTangent1->add_component<CurveControlPoint>()->set_is_left_control_point(true);
		leftTangent1->m_localTr.m_position = glm::vec3(0.020f, 0.0f, 0.914f);

		SceneNode* rightTangent1 = curvePoint1->create_child("Right Control Point 1");
		rightTangent1->add_component<CurveControlPoint>()->set_is_left_control_point(false);
		rightTangent1->m_localTr.m_position = glm::vec3(-0.281f, 0.0f, -1.368f);


		// Add the control points for point 2
		SceneNode* leftTangent2 = curvePoint2->create_child("Left Control Point 2");
		leftTangent2->add_component<CurveControlPoint>()->set_is_left_control_point(true);
		leftTangent2->m_localTr.m_position = glm::vec3(-2.104f, -1.546f, -0.447f);

		SceneNode* rightTangent2 = curvePoint2->create_child("Right Control Point 2");
		rightTangent2->add_component<CurveControlPoint>()->set_is_left_control_point(false);
		rightTangent2->m_localTr.m_position = glm::vec3(0.226, 0.0f, -1.104f);


		// Set the curve points' positions
		curvePoint0->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		curvePoint1->m_localTr.m_position = glm::vec3(2.325f, 0.0f, 0.736f);
		curvePoint2->m_localTr.m_position = glm::vec3(6.313f, 0.0f, -5.033f);


		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(-2.2f, 1.2f, 2.0f));
			editorCam->set_target(editorCam->get_position() + glm::vec3(1.0f, -0.3f, -1.0f) * 25.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on bezier curve demo\n";
	}

	void MainMenuBarGUI::load_skinned_animation_scene()
	{
		load_empty_scene();

		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		// Create the nodes
		SceneNode* brainStem = root->create_child("BRAIN STEM");
		SceneNode* cesiumMan = root->create_child("CESIUM MAN");
		SceneNode* fox = root->create_child("FOX");
		SceneNode* box = root->create_child("BOX");
		SceneNode* cube = root->create_child("CUBE");


		// Add the models
		ModelInstance* brainStemInstance = brainStem->add_component<ModelInstance>();
		brainStemInstance->change_model("data/Models/BrainStem/BrainStem.gltf");

		ModelInstance* cesiumManInstance = cesiumMan->add_component<ModelInstance>();
		cesiumManInstance->change_model("data/Models/rigged figure/CesiumMan.gltf");

		ModelInstance* foxInstance = fox->add_component<ModelInstance>();
		foxInstance->change_model("data/Models/Fox/Fox.gltf");

		ModelInstance* boxInstance = box->add_component<ModelInstance>();
		boxInstance->change_model("data/Models/BoxAnimated/BoxAnimated.gltf");

		ModelInstance* cubeInstance = cube->add_component<ModelInstance>();
		cubeInstance->change_model("data/Models/CubeAnimated/AnimatedCube.gltf");


		// Set their animations
		AnimationReference* brainStemAnim = brainStem->get_component<AnimationReference>();
		brainStemAnim->change_animation(0, "");
		brainStemAnim->set_anim_time_scale(0.8f);

		AnimationReference* cesiumManAnim = cesiumMan->get_component<AnimationReference>();
		cesiumManAnim->change_animation(0, "");
		cesiumManAnim->set_anim_time_scale(2.15f);

		AnimationReference* foxAnim = fox->get_component<AnimationReference>();
		foxAnim->change_animation(2, "Run");
		foxAnim->set_anim_time_scale(2.25f);

		AnimationReference* boxAnim = box->get_component<AnimationReference>();
		boxAnim->change_animation(0, "");
		boxAnim->set_anim_time_scale(3.0f);

		AnimationReference* cubeAnim = cube->get_component<AnimationReference>();
		cubeAnim->change_animation(0, "animation_AnimatedCube");
		cubeAnim->set_anim_time_scale(0.5f);


		// Set their local transforms
		brainStem->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		cesiumMan->m_localTr.m_position = glm::vec3(2.206f, 0.0f, 0.0f);
		fox->m_localTr.m_position = glm::vec3(4.312f, 0.0f, 0.0f);
		fox->m_localTr.m_scale = glm::vec3(0.022f, 0.022f, 0.022f);
		box->m_localTr.m_position = glm::vec3(0.0f, 0.0f, -8.094f);
		cube->m_localTr.m_position = glm::vec3(4.11f, 0.0f, -8.631f);
		cube->m_localTr.m_scale = glm::vec3(0.64f, 0.64f, 0.64f);


		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(2.206f, 1.0f, 7.0f));
			editorCam->set_target(editorCam->get_position() + glm::vec3(0.0f, 0.0f, -1.0f) * 25.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on skinned animations demo\n";
	}

	void MainMenuBarGUI::load_nested_models_scene()
	{
		load_empty_scene();

		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		// Create the xbot node
		SceneNode* xbot = root->create_child("XBOT");

		// Add the xbot model
		ModelInstance* xbotInstance = xbot->add_component<ModelInstance>();
		xbotInstance->change_model("data/Models/xbot/xbot.gltf");


		// Get the right hand joint node to place the axe
		SceneNode* rightShoulder = xbot->get_children().at(0)->get_children().at(2)->get_children().at(0)->get_children().at(0)->get_children().at(0)->get_children().at(2);
		SceneNode* rightHand = rightShoulder->get_children().at(0)->get_children().at(0)->get_children().at(0);

		// Create the actual axe as child of the right hand
		SceneNode* axe = rightHand->create_child("AXE");

		// Add the axe model
		ModelInstance* axeInstance = axe->add_component<ModelInstance>();
		axeInstance->change_model("data/Models/axe/scene.gltf");


		// Set the local transform of the axe
		axe->m_localTr.m_position = glm::vec3(-27.708f, -3.855f, 58.506f);
		axe->m_localTr.m_orientation = glm::quat(-0.483f, 0.318f, 0.666f, 0.471f);
		axe->m_localTr.m_scale = glm::vec3(0.34f, 0.34f, 0.34f);


		// Set the animation
		AnimationReference* xbotAnim = xbot->get_component<AnimationReference>();
		xbotAnim->change_animation(8, "KICK");
		xbotAnim->set_anim_time_scale(1.0f);


		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(1.0f, 1.0f, 5.0f));
			editorCam->set_target(editorCam->get_position() + glm::normalize(glm::vec3(-0.3f, 0.0f, -1.0f)) * 25.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on skinned animations demo\n";
	}

	void MainMenuBarGUI::load_path_following_scene()
	{
		load_empty_scene();

		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		// Create the curve node with a PiecewiseCurve component
		SceneNode* curveNode = root->create_child("BEZIER PIECEWISE CURVE");
		EditorState& editorState = EditorState::get_main_editor_state();
		editorState.m_selectedNode = curveNode;
		PiecewiseCurve* curveComp = curveNode->add_component<PiecewiseCurve>();
		curveComp->initialize();
		curveComp->change_curve_type(CURVE_TYPE::BEZIER);
		curveComp->change_finish_mode(PiecewiseCurve::FINISH_MODE::PINPONG);

		// Create the curve point nodes
		SceneNode* curvePoint0 = curveNode->create_child("Curve Point 0");
		SceneNode* curvePoint1 = curveNode->create_child("Curve Point 1");
		SceneNode* curvePoint2 = curveNode->create_child("Curve Point 2");

		// Add to them the curve point components (and set their times)
		curvePoint0->add_component<CurvePoint>();
		curvePoint1->add_component<CurvePoint>()->set_time(2.0f);
		curvePoint2->add_component<CurvePoint>()->set_time(4.0f);


		// Add the control points for point 0
		SceneNode* leftTangent0 = curvePoint0->create_child("Left Control Point 0");
		leftTangent0->add_component<CurveControlPoint>()->set_is_left_control_point(true);
		leftTangent0->m_localTr.m_position = glm::vec3(-0.666f, 0.0f, 0.504f);

		SceneNode* rightTangent0 = curvePoint0->create_child("Right Control Point 0");
		rightTangent0->add_component<CurveControlPoint>()->set_is_left_control_point(false);
		rightTangent0->m_localTr.m_position = glm::vec3(1.541f, 0.0f, -1.616f);


		// Add the control points for point 1
		SceneNode* leftTangent1 = curvePoint1->create_child("Left Control Point 1");
		leftTangent1->add_component<CurveControlPoint>()->set_is_left_control_point(true);
		leftTangent1->m_localTr.m_position = glm::vec3(-1.362f, 0.0f, 1.7f);

		SceneNode* rightTangent1 = curvePoint1->create_child("Right Control Point 1");
		rightTangent1->add_component<CurveControlPoint>()->set_is_left_control_point(false);
		rightTangent1->m_localTr.m_position = glm::vec3(1.509f, 0.0f, -1.892f);


		// Add the control points for point 2
		SceneNode* leftTangent2 = curvePoint2->create_child("Left Control Point 2");
		leftTangent2->add_component<CurveControlPoint>()->set_is_left_control_point(true);
		leftTangent2->m_localTr.m_position = glm::vec3(0.317f, 0.0f, -1.421f);

		SceneNode* rightTangent2 = curvePoint2->create_child("Right Control Point 2");
		rightTangent2->add_component<CurveControlPoint>()->set_is_left_control_point(false);
		rightTangent2->m_localTr.m_position = glm::vec3(0.551f, 0.0f, 0.769f);


		// Set the curve points' positions
		curvePoint0->m_localTr.m_position = glm::vec3(-3.253f, 0.0f, 17.762f);
		curvePoint1->m_localTr.m_position = glm::vec3(-0.137f, 0.0f, 18.872f);
		curvePoint2->m_localTr.m_position = glm::vec3(3.0f, 0.0f, 18.0f);


		// Place the camera
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		if (editorCam)
		{
			editorCam->set_position(glm::vec3(0.0f, 2.0f, 25.0f));
			editorCam->set_target(editorCam->get_position() + glm::vec3(0.0f, -0.2f, -1.0f) * 30.0f);
		}
		else
			std::cout << "ERROR: Editor camera not being used on path following demo\n";
	}

	void MainMenuBarGUI::load_blending_1d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = true;
		scene.change_camera(false);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		SphericalCamera* sphericalCam = dynamic_cast<SphericalCamera*>(cam);

		// Create the nodes
		SceneNode* xBot = root->create_child("X-BOT");
		sphericalCam->set_focal_node(xBot);
		sphericalCam->set_focal_offset(glm::vec3(0.0f, 1.5f, 0.0f));


		// Add the model
		ModelInstance* xBotInstance = xBot->add_component<ModelInstance>();
		xBotInstance->change_model("data/Models/xbot/xbot.gltf");


		// Set the animation component and blend tree usage
		AnimationReference* xBotAnim = xBot->get_component<AnimationReference>();
		xBotAnim->set_blend_tree_type(1);

		// Get the blend tree and build it;
		Blend1D* blend1d = dynamic_cast<Blend1D*>(xBotAnim->get_blend_tree());
		blend1d->m_blendParam = 0.0f;

		BlendAnim* blendAnim1 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[0]);
		BlendAnim* blendAnim2 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[1]);
		BlendAnim* blendAnim3 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));
		BlendAnim* blendAnim4 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));
		BlendAnim* blendAnim5 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));

		blendAnim1->m_blendPos.x = 0.0f;
		blendAnim2->m_blendPos.x = 0.5f;
		blendAnim3->m_blendPos.x = 1.0f;
		blendAnim4->m_blendPos.x = 1.5f;
		blendAnim5->m_blendPos.x = 2.0f;

		// 14(RUMBA-DANCING), 5(IDLE), 22(WALK), 6(JOG), 15(RUN), 3(FAST RUN)
		blendAnim1->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[4]);
		blendAnim2->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[22]);
		blendAnim3->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[6]);
		blendAnim4->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[15]);
		blendAnim5->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[3]);
		
		
		// Add the player controller script component
		xBot->add_component<PlayerController>();
	}

	void MainMenuBarGUI::load_blending_2d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = true;
		scene.change_camera(false);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(false);
		SphericalCamera* sphericalCam = dynamic_cast<SphericalCamera*>(cam);

		// Create the nodes
		SceneNode* xBot = root->create_child("X-BOT");
		xBot->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 20.0f);
		sphericalCam->set_focal_node(xBot);
		sphericalCam->set_focal_offset(glm::vec3(0.0f, 1.0f, 0.0f));


		// Add the model
		ModelInstance* xBotInstance = xBot->add_component<ModelInstance>();
		xBotInstance->change_model("data/Models/xbot/xbot.gltf");


		// Set the animation component and blend tree usage
		AnimationReference* xBotAnim = xBot->get_component<AnimationReference>();
		xBotAnim->set_blend_tree_type(2);

		// Get the blend tree and build it;
		Blend2D* blend2d = dynamic_cast<Blend2D*>(xBotAnim->get_blend_tree());
		blend2d->m_blendParam = glm::vec2(0.0f, 0.0f);

		BlendAnim* blendAnim1 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[0]);
		BlendAnim* blendAnim2 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[1]);
		BlendAnim* blendAnim3 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[2]);
		BlendAnim* blendAnim4 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));
		BlendAnim* blendAnim5 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));

		blendAnim1->m_blendPos = glm::vec2(0.0f, 0.0f);
		blendAnim2->m_blendPos = glm::vec2(0.0f, 1.0f);
		blendAnim3->m_blendPos = glm::vec2(-1.0f, 0.0f);
		blendAnim4->m_blendPos = glm::vec2(1.0f, 0.0f);
		blendAnim5->m_blendPos = glm::vec2(0.0f, -1.0f);

		// 14(RUMBA-DANCING), 4(HIP HOP DANCING), 22(WALK), 23(WALK-LEFT), 24(WALK-RIGHT), 25(WALK-BACK)
		blendAnim1->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[14]);
		blendAnim2->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[22]);
		blendAnim3->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[23]);
		blendAnim4->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[24]);
		blendAnim5->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[25]);


		// Add the player controller script component
		xBot->add_component<PlayerController>();
	}

	void MainMenuBarGUI::load_blend_editor_1d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);

		// Create the nodes
		SceneNode* xBot = root->create_child("X-BOT");
		xBot->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 0.0f);


		// Add the model
		ModelInstance* xBotInstance = xBot->add_component<ModelInstance>();
		xBotInstance->change_model("data/Models/xbot/xbot.gltf");


		// Set the animation component and blend tree usage
		AnimationReference* xBotAnim = xBot->get_component<AnimationReference>();
		xBotAnim->set_blend_tree_type(1);

		// Get the blend tree and build it;
		Blend1D* blend1d = dynamic_cast<Blend1D*>(xBotAnim->get_blend_tree());
		blend1d->m_blendParam = 0.0f;

		BlendAnim* blendAnim1 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[0]);
		BlendAnim* blendAnim2 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[1]);
		BlendAnim* blendAnim3 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));
		BlendAnim* blendAnim4 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));
		BlendAnim* blendAnim5 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));

		blendAnim1->m_blendPos.x = 0.0f;
		blendAnim2->m_blendPos.x = 0.5f;
		blendAnim3->m_blendPos.x = 1.0f;
		blendAnim4->m_blendPos.x = 1.5f;
		blendAnim5->m_blendPos.x = 2.0f;

		// 14(RUMBA-DANCING), 4(HIP HOP DANCING), 5(IDLE), 22(WALK), 6(JOG), 15(RUN), 3(FAST RUN)
		blendAnim1->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[4]);
		blendAnim2->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[22]);
		blendAnim3->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[6]);
		blendAnim4->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[15]);
		blendAnim5->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[3]);


		// Place the camera
		editorCam->set_position(glm::vec3(0.0f, 2.0f, 5.0f));
		editorCam->set_target(editorCam->get_position() + glm::vec3(0.0f, -0.1f, -1.0f) * 25.0f);
	}

	void MainMenuBarGUI::load_blend_editor_2d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);

		// Create the nodes
		SceneNode* xBot = root->create_child("X-BOT");
		xBot->m_localTr.m_position = glm::vec3(0.0f, 0.0f, 0.0f);


		// Add the model
		ModelInstance* xBotInstance = xBot->add_component<ModelInstance>();
		xBotInstance->change_model("data/Models/xbot/xbot.gltf");


		// Set the animation component and blend tree usage
		AnimationReference* xBotAnim = xBot->get_component<AnimationReference>();
		xBotAnim->set_blend_tree_type(2);

		// Get the blend tree and build it;
		Blend2D* blend2d = dynamic_cast<Blend2D*>(xBotAnim->get_blend_tree());
		blend2d->m_blendParam = glm::vec2(0.0f, 0.0f);

		BlendAnim* blendAnim1 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[0]);
		BlendAnim* blendAnim2 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[1]);
		BlendAnim* blendAnim3 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->m_children[2]);
		BlendAnim* blendAnim4 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));
		BlendAnim* blendAnim5 = static_cast<BlendAnim*>(xBotAnim->get_blend_tree()->add_child(BlendNodeTypes::BLEND_ANIM));

		blendAnim1->m_blendPos = glm::vec2(0.0f, 0.0f);
		blendAnim2->m_blendPos = glm::vec2(0.0f, 1.0f);
		blendAnim3->m_blendPos = glm::vec2(-1.0f, 0.0f);
		blendAnim4->m_blendPos = glm::vec2(1.0f, 0.0f);
		blendAnim5->m_blendPos = glm::vec2(0.0f, -1.0f);

		// 14(RUMBA-DANCING), 4(HIP HOP DANCING), 22(WALK), 23(WALK-LEFT), 24(WALK-RIGHT), 25(WALK-BACK)
		blendAnim1->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[14]);
		blendAnim2->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[22]);
		blendAnim3->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[23]);
		blendAnim4->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[24]);
		blendAnim5->m_animSource = &(xBotInstance->get_owner()->get_model()->m_animations[25]);


		// Place the camera
		editorCam->set_position(glm::vec3(0.0f, 2.0f, 5.0f));
		editorCam->set_target(editorCam->get_position() + glm::vec3(0.0f, -0.1f, -1.0f) * 25.0f);
	}


	void MainMenuBarGUI::load_ik_analytical_2d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);

		// Place the camera
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		editorCam->set_position(glm::vec3(0.0f, 0.0f, 20.0f));
		editorCam->set_target(glm::vec3(0.0f, 0.0f, 0.0f));


		// Create the ik chain joints
		SceneNode* joint0 = root->create_child("CHAIN ROOT");
		SceneNode* joint1 = joint0->create_child("Joint 1");
		SceneNode* joint2 = joint1->create_child("Joint 2");
		SceneNode* target = root->create_child("TARGET");

		// Place the joints
		joint1->m_localTr.m_position.x = 2.0f;
		joint2->m_localTr.m_position.x = 4.0f;
		target->m_localTr.m_position.x = 6.0f;


		// Setup the ik chain component
		IKChainRoot* chainRoot = joint0->add_component<IKChainRoot>();
		chainRoot->initialize();
		chainRoot->set_end_effector(joint2);
		chainRoot->set_target(target);


		// Make the target the selected node in the editor
		EditorState& editorState = EditorState::get_main_editor_state();
		editorState.m_selectedNode = target;
	}

	void MainMenuBarGUI::load_ik_ccd_3d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);

		// Place the camera
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		editorCam->set_position(glm::vec3(0.0f, 0.0f, 25.0f));
		editorCam->set_target(glm::vec3(0.0f, 0.0f, 0.0f));


		// Create the ik chain joints
		SceneNode* joint0 = root->create_child("CHAIN ROOT");
		SceneNode* joint1 = joint0->create_child("Joint 1");
		SceneNode* joint2 = joint1->create_child("Joint 2");
		SceneNode* joint3 = joint2->create_child("Joint 3");
		SceneNode* joint4 = joint3->create_child("Joint 4");
		SceneNode* target = root->create_child("TARGET");

		// Place the joints
		joint1->m_localTr.m_position.x = 2.0f;
		joint2->m_localTr.m_position.x = 2.0f;
		joint3->m_localTr.m_position.x = 2.0f;
		joint4->m_localTr.m_position.x = 2.0f;
		target->m_localTr.m_position.x = 8.0f;


		// Setup the ik chain component
		IKChainRoot* chainRoot = joint0->add_component<IKChainRoot>();
		chainRoot->initialize();
		chainRoot->set_end_effector(joint4);
		chainRoot->set_target(target);
		chainRoot->set_solver_type(IKSolverType::CCD_3D);


		// Make the target the selected node in the editor
		EditorState& editorState = EditorState::get_main_editor_state();
		editorState.m_selectedNode = target;
	}

	void MainMenuBarGUI::load_ik_fabrik_3d_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);

		// Place the camera
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		editorCam->set_position(glm::vec3(0.0f, 0.0f, 25.0f));
		editorCam->set_target(glm::vec3(0.0f, 0.0f, 0.0f));


		// Create the ik chain joints
		SceneNode* joint0 = root->create_child("CHAIN ROOT");
		SceneNode* joint1 = joint0->create_child("Joint 1");
		SceneNode* joint2 = joint1->create_child("Joint 2");
		SceneNode* joint3 = joint2->create_child("Joint 3");
		SceneNode* joint4 = joint3->create_child("Joint 4");
		SceneNode* target = root->create_child("TARGET");

		// Place the joints
		joint1->m_localTr.m_position.x = 2.0f;
		joint2->m_localTr.m_position.x = 2.0f;
		joint3->m_localTr.m_position.x = 2.0f;
		joint4->m_localTr.m_position.x = 2.0f;
		target->m_localTr.m_position.x = 8.0f;


		// Setup the ik chain component
		IKChainRoot* chainRoot = joint0->add_component<IKChainRoot>();
		chainRoot->initialize();
		chainRoot->set_end_effector(joint4);
		chainRoot->set_target(target);
		chainRoot->set_solver_type(IKSolverType::FABRIK_3D);


		// Make the target the selected node in the editor
		EditorState& editorState = EditorState::get_main_editor_state();
		editorState.m_selectedNode = target;
	}


	void MainMenuBarGUI::load_ik_on_skeleton_scene()
	{
		// Clear the scene
		load_empty_scene();

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		DebugRenderer::s_enableSkeletonDrawing = true;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);

		// Place the camera
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		editorCam->set_position(glm::vec3(0.0f, 1.0f, 3.5f));
		editorCam->set_target(glm::vec3(0.0f, 1.0f, -25.0f));


		// Create the model
		SceneNode* brainStem = root->create_child("BRAIN_STEM");
		ModelInstance* modelInst = brainStem->add_component<ModelInstance>();
		modelInst->change_model("data/Models/BrainStem/BrainStem.gltf");

		// Set the chain root and end effector from the model nodes
		SceneNode* leftHip = brainStem->get_children().at(0)->get_children().at(0)->get_children().at(0)->get_children().at(0)->get_children().at(1);
		leftHip->change_name("CHAIN ROOT");
		IKChainRoot* chainRootComp = leftHip->add_component<IKChainRoot>();
		chainRootComp->initialize();
		SceneNode* leftFoot = leftHip->get_children().at(0)->get_children().at(0)->get_children().at(0);
		leftFoot->change_name("END EFFECTOR");
		chainRootComp->set_end_effector(leftFoot);
		chainRootComp->set_solver_type(IKSolverType::FABRIK_3D);

		// Create and set the target of the ik chain
		SceneNode* target = root->create_child("TARGET");
		target->m_localTr.m_position = glm::vec3(-0.12f, 0.05f, 0.0f);
		chainRootComp->set_target(target);

		// Make the target the selected node in the editor
		EditorState& editorState = EditorState::get_main_editor_state();
		editorState.m_selectedNode = target;
	}


	void MainMenuBarGUI::load_cloth_simulation_scene()
	{
		// Clear the scene
		load_empty_scene();

		Renderer::get_instance().get_skybox()->set_active(false);

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		scene.m_lightProperties.m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		DebugRenderer::s_enableSkeletonDrawing = true;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);

		// Place the camera
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		editorCam->set_position(glm::vec3(0.0f, 0.0f, 10.0f));
		editorCam->set_target(glm::vec3(0.0f, 0.0f, -15.0f));


		// Place the object with the cloth
		SceneNode* clothNode = root->create_child("CLOTH");
		Cloth* clothComp = clothNode->add_component<Cloth>();
		clothComp->initialize();


		// Make the cloth node the selected one in the editor
		//EditorState& editorState = EditorState::get_main_editor_state();
		//editorState.m_selectedNode = clothNode;
	}


	void MainMenuBarGUI::load_cloth_collision_scene()
	{
		// Clear the scene
		load_empty_scene();

		Renderer::get_instance().get_skybox()->set_active(false);

		ResourceManager& resourceMgr = ResourceManager::get_instance();
		Scene& scene = Scene::get_instance();
		scene.m_lightProperties.m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
		SceneNode* root = scene.get_root();

		DebugRenderer::s_enableGridDrawing = false;
		DebugRenderer::s_enableSkeletonDrawing = true;
		scene.change_camera(true);
		ICamera* cam = scene.get_active_camera();
		cam->set_is_active(true);

		// Place the camera
		EditorCamera* editorCam = dynamic_cast<EditorCamera*>(cam);
		editorCam->set_position(glm::vec3(0.0f, 0.0f, 10.0f));
		editorCam->set_target(glm::vec3(0.0f, 0.0f, -15.0f));


		// Place the sphere to check collision against
		SceneNode* sphereNode = root->create_child("Sphere");
		ModelInstance* modelComp = sphereNode->add_component<ModelInstance>();
		modelComp->change_model("data/Models/Sphere/Sphere.gltf");
		sphereNode->m_localTr.m_position = glm::vec3(1.6f, -2.5f, 0.0f);
		sphereNode->m_localTr.m_scale = glm::vec3(0.7f, 0.7f, 0.7f);

		// Place the object with the cloth
		SceneNode* clothNode = root->create_child("CLOTH");
		Cloth* clothComp = clothNode->add_component<Cloth>();
		clothComp->initialize(sphereNode->get_children().front());
		clothComp->get_system().m_constraintIterations = 20;


		// Make the cloth node the selected one in the editor
		EditorState& editorState = EditorState::get_main_editor_state();
		editorState.m_selectedNode = sphereNode;
	}
}