/**
* @file Analytic2Bone2DSolver.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Solves ik problems in 2d involving only 3 joints (2 bones).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Analytic2Bone2DSolver.h"
#include "IKChain.h"
#include "Composition/SceneNode.h"


namespace cs460
{
	Analytic2Bone2DSolver::Analytic2Bone2DSolver()
	{
	}

	Analytic2Bone2DSolver::~Analytic2Bone2DSolver()
	{
	}


	// Tries to solve the internal IK chain
	IKSolverStatus Analytic2Bone2DSolver::solve()
	{
		m_status = IKSolverStatus::PROCESSING;

		SceneNode* chainRoot = m_chain->get_chain_root();
		SceneNode* endEffector = m_chain->get_end_effector();
		SceneNode* target = m_chain->get_target();
		if (chainRoot == nullptr || endEffector == nullptr || target == nullptr)
		{
			std::cout << "ERROR : ANALYTIC 2 BONE 2D SOLVER : Either chain root, end effector or target are not present!!!\n";
			m_status = IKSolverStatus::FAILURE;
			return m_status;
		}

		float endX = target->m_worldTr.m_position.x;
		float endY = target->m_worldTr.m_position.y;
		float d[2] = { 1.0f, 1.0f };

		// Get the distances of the 2 bones (d array)
		int i = 0;
		SceneNode* traverser = endEffector;
		while (i < 2)
		{
			if (traverser == nullptr || traverser == chainRoot)
			{
				std::cout << "ERROR : ANALYTIC 2 BONE 2D SOLVER : Joint hierarchy not correctly setup!!!\n";
				m_status = IKSolverStatus::FAILURE;
				return m_status;
			}

			const glm::vec3& pos = traverser->m_worldTr.m_position;
			const glm::vec3& parentPos = traverser->get_parent()->m_worldTr.m_position;
			const glm::vec2& boneVec = glm::vec2(pos) - glm::vec2(parentPos);

			d[1 - i] = glm::length(boneVec);

			traverser = traverser->get_parent();
			++i;
		}


		// Compute the local orientation of the closest bone to the end effector
		float numerator = endX * endX + endY * endY - (d[0] * d[0] + d[1] * d[1]);
		float denominator = 2 * d[0] * d[1];

		// Check for possible nan
		if (glm::epsilonEqual(denominator, 0.0f, FLT_EPSILON))
			denominator = 0.001f;

		float cosTheta2 = numerator / denominator;

		// Check if the method will fail
		if (cosTheta2 < -1)
		{
			std::cout<<"INFO : ANALYTIC 2 BONE 2D SOLVER : Trying to bend farther than possible\n";
			m_status = IKSolverStatus::FAILURE;
			return m_status;
		}
		else if (cosTheta2 > 1)
		{
			std::cout << "INFO : ANALYTIC 2 BONE 2D SOLVER : Trying to extend farther than possible\n";
			m_status = IKSolverStatus::FAILURE;
			return m_status;
		}

		// We can negate this value to get another solution
		float theta2 = acos(cosTheta2);


		// Compute the local orientation of the closest bone to the chain root
		numerator = endY * (d[0] + d[1] * cosTheta2) - endX * (d[1] * sin(theta2));
		denominator = endX * (d[0] + d[1] * cosTheta2) + endY * (d[1] * sin(theta2));

		// Check for possible nan
		if (glm::epsilonEqual(denominator, 0.0f, FLT_EPSILON))
			denominator = 0.001f;

		float theta1 = atan2(numerator, denominator);


		// Apply the computed local rotations
		glm::quat localRot1(glm::vec3(0.0f, 0.0f, theta1));
		glm::quat localRot2(glm::vec3(0.0f, 0.0f, theta2));
		endEffector->get_parent()->m_localTr.m_orientation = localRot2;
		chainRoot->m_localTr.m_orientation = localRot1;

		m_status = IKSolverStatus::SUCCESS;
		return m_status;
	}

	// Resets all of this solver's data
	void Analytic2Bone2DSolver::reset_solver()
	{
		m_status = IKSolverStatus::IDLE;
	}
}