/**
* @file IKChainRoot.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Component that represents an ik chain (only added to the root of the chain).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "IKChainRoot.h"
#include "Animation/InverseKinematics/IKChain.h"
#include "Animation/InverseKinematics/Analytic2Bone2DSolver.h"
#include "Animation/InverseKinematics/CCD3DSolver.h"
#include "Animation/InverseKinematics/FABRIK3DSolver.h"
#include "Composition/SceneNode.h"
#include "Animation/Animator.h"
#include "Graphics/Systems/DebugRenderer.h"


namespace cs460
{
	IKChainRoot::IKChainRoot()
	{
		m_chain = new IKChain;
		m_solver = new Analytic2Bone2DSolver;

		Animator::get_instance().add_ik_chain(this);
	}

	IKChainRoot::~IKChainRoot()
	{
		delete m_chain;
		delete m_solver;

		Animator::get_instance().remove_ik_chain(this);
	}


	void IKChainRoot::initialize()
	{
		m_chain->set_chain_root(get_owner());
		m_solver->set_ik_chain(m_chain);
	}

	IKSolverStatus IKChainRoot::update()
	{
		if (m_solver == nullptr)
		{
			m_lastSolverStatus = IKSolverStatus::IDLE;
			return m_lastSolverStatus;
		}

		
		// Don't solve if we have finished processing and the target hasn't moved
		SceneNode* target = m_chain->get_target();
		const glm::vec3& currentTargetPos = target->m_worldTr.m_position;
		if (m_solver->get_status() != IKSolverStatus::PROCESSING && glm::all(glm::epsilonEqual(currentTargetPos, m_lastTargetPos, FLT_EPSILON)))
		{
			m_lastSolverStatus = m_solver->get_status();//IKSolverStatus::IDLE;
			return m_lastSolverStatus;
		}
		
		// Update the last target position if the previous frame's solve was successful
		if (m_solver->get_status() != IKSolverStatus::PROCESSING)
			m_lastTargetPos = target->m_worldTr.m_position;

		// Solve using the internal solver
		m_lastSolverStatus = m_solver->solve();
		return m_lastSolverStatus;
	}

	void IKChainRoot::debug_draw()
	{
		if (!m_drawChain)
			return;

		glm::vec4 boneColor{ 0.0f, 1.0f, 0.0f, 1.0f };
		if (m_lastSolverStatus == IKSolverStatus::IDLE)
			boneColor = DebugRenderer::s_ikBoneColorIdle;
		else if (m_lastSolverStatus == IKSolverStatus::PROCESSING)
			boneColor = DebugRenderer::s_ikBoneColorProcessing;
		else if (m_lastSolverStatus == IKSolverStatus::SUCCESS)
			boneColor = DebugRenderer::s_ikBoneColorSuccess;
		else if (m_lastSolverStatus == IKSolverStatus::FAILURE)
			boneColor = DebugRenderer::s_ikBoneColorFailure;

		DebugRenderer::draw_ik_chain(m_chain, boneColor, DebugRenderer::s_ikBoneHighlightColor, DebugRenderer::s_ikJointColor, DebugRenderer::s_ikTargetColor);
	}


	void IKChainRoot::set_end_effector(SceneNode* endEffector)
	{
		if (m_chain == nullptr)
			return;

		m_chain->set_end_effector(endEffector);
	}

	void IKChainRoot::set_target(SceneNode* target)
	{
		if (m_chain == nullptr)
			return;

		m_chain->set_target(target);
	}

	void IKChainRoot::set_solver_type(IKSolverType type)
	{
		if (m_solverType == type)
			return;

		m_solverType = type;

		delete m_solver;
		m_solver = nullptr;

		if (type == IKSolverType::ANALYTIC_2BONE_2D)
			m_solver = new Analytic2Bone2DSolver;
		else if (type == IKSolverType::CCD_3D)
			m_solver = new CCD3DSolver;
		else if (type == IKSolverType::FABRIK_3D)
			m_solver = new FABRIK3DSolver;

		m_solver->set_ik_chain(m_chain);
	}


	IKChain* IKChainRoot::get_chain() const
	{
		return m_chain;
	}
	IKSolver* IKChainRoot::get_solver() const
	{
		return m_solver;
	}
	IKSolverType IKChainRoot::get_solver_type() const
	{
		return m_solverType;
	}


	void IKChainRoot::on_gui()
	{
		ImGui::Checkbox("Draw IK Chain", &m_drawChain);

		std::string previewValue = "Analytic 2 Bone 2D";
		if (m_solverType == IKSolverType::ANALYTIC_2BONE_2D)
			previewValue = "Analytic 2 Bone 2D";
		else if (m_solverType == IKSolverType::CCD_3D)
			previewValue = "CCD 3D";
		else if (m_solverType == IKSolverType::FABRIK_3D)
			previewValue = "FABRIK 3D";

		if (ImGui::BeginCombo("Solver Type", previewValue.c_str()))
		{
			if (ImGui::Selectable("Analytic 2 Bone 2D"))
			{
				m_solverType = IKSolverType::ANALYTIC_2BONE_2D;
			}
			if (ImGui::Selectable("CCD 3D"))
			{
				m_solverType = IKSolverType::CCD_3D;
			}
			if (ImGui::Selectable("FABRIK 3D"))
			{
				m_solverType = IKSolverType::FABRIK_3D;
			}

			ImGui::EndCombo();
		}
	}
}