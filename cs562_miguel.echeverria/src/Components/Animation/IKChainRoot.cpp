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
			return IKSolverStatus::IDLE;

		
		// Don't solve if we have finished processing and the end effector hasn't moved
		SceneNode* endEffector = m_chain->get_end_effector();
		const glm::vec2& currentPos = glm::vec2(endEffector->m_localTr.m_position);
		if (m_solver->get_status() != IKSolverStatus::PROCESSING && glm::all(glm::epsilonEqual(currentPos, m_lastEndEffectorLocalPos, FLT_EPSILON)))
			return IKSolverStatus::IDLE;

		// Update the last end effector local position if the previous frame's solve was successful
		if (m_solver->get_status() == IKSolverStatus::SUCCESS)
			m_lastEndEffectorLocalPos = glm::vec2(endEffector->m_localTr.m_position);

		// Solve using the internal solver
		return m_solver->solve();
	}

	void IKChainRoot::debug_draw(const glm::vec4& boneColor, const glm::vec4& boneHighlightColor, const glm::vec4& jointColor, const glm::vec4& endEffectorColor)
	{
		if (!m_drawChain)
			return;

		DebugRenderer::draw_ik_chain(m_chain, boneColor, boneHighlightColor, jointColor, endEffectorColor);
	}


	void IKChainRoot::set_end_effector(SceneNode* endEffector)
	{
		if (m_chain == nullptr)
			return;

		m_chain->set_end_effector(endEffector);
		m_lastEndEffectorLocalPos = endEffector->m_localTr.m_position;
	}

	IKChain* IKChainRoot::get_chain() const
	{
		return m_chain;
	}
	IKSolver* IKChainRoot::get_solver() const
	{
		return m_solver;
	}


	void IKChainRoot::on_gui()
	{
		ImGui::Checkbox("Draw IK Chain", &m_drawChain);
	}
}