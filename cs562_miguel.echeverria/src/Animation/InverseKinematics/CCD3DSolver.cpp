/**
* @file CCD3DSolver.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Solves ik problems in 3d using the Cyclic Coordinate Descent method.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "CCD3DSolver.h"
#include "IKChain.h"
#include "Composition/SceneNode.h"
#include "Graphics/Systems/DebugRenderer.h"
#include "Math/Geometry/Geometry.h"
#include <GL/glew.h>


namespace cs460
{
	CCD3DSolver::CCD3DSolver()
	{
	}

	CCD3DSolver::~CCD3DSolver()
	{
	}


	// Tries to solve the internal IK chain
	IKSolverStatus CCD3DSolver::solve()
	{
		m_status = IKSolverStatus::PROCESSING;

		SceneNode* chainRoot = m_chain->get_chain_root();
		SceneNode* endEffector = m_chain->get_end_effector();
		SceneNode* target = m_chain->get_target();
		if (chainRoot == nullptr || endEffector == nullptr || target == nullptr)
		{
			std::cout << "ERROR : CCD 3D SOLVER : Either chain root, end effector or target are not present!!!\n";
			m_status = IKSolverStatus::FAILURE;
			return m_status;
		}


		// Get the target world positions
		const glm::vec3& targetWorldPos = target->m_worldTr.m_position;

		// Process until a solution is found, or max iterations is reached
		for (unsigned i = 0; i < m_maxIterations; ++i)
		{
			// For each joint from the parent of the end effector until the chain root
			SceneNode* traverser = endEffector->get_parent();
			while (traverser != nullptr && traverser != chainRoot->get_parent())
			{
				const glm::vec3& endWorldPos = endEffector->m_worldTr.m_position;

				// Do ccd and update the nodes' rotations
				apply_local_rotation(traverser, endWorldPos, targetWorldPos);
				update_world_transforms(traverser, endEffector);
				
				traverser = traverser->get_parent();
			}

			// Check if we found the solution, and if not repeat
			if (check_solution(endEffector->m_worldTr.m_position, targetWorldPos))
			{
				m_status = IKSolverStatus::SUCCESS;
				return m_status;
			}
		}
			
		m_status = IKSolverStatus::FAILURE;
		return m_status;
	}

	// Resets all of this solver's data
	void CCD3DSolver::reset_solver()
	{
		m_status = IKSolverStatus::IDLE;
	}


	// Main part of the CCD algorithm, uses the end effector and target's world positions to compute the local rotation of currNode
	void CCD3DSolver::apply_local_rotation(SceneNode* currNode, const glm::vec3& endWorldPos, const glm::vec3& targetWorldPos)
	{
		// Get the vectors v1 = endEffector - curr; and v2 = target - curr
		// (curr being the world position of the current joint)
		const glm::vec3& currWorldPos = currNode->m_worldTr.m_position;
		const glm::vec3& v1 = endWorldPos - currWorldPos;
		const glm::vec3& v2 = targetWorldPos - currWorldPos;

		// Normalize v1 and v2
		const glm::vec3& v1Norm = glm::normalize(v1);
		const glm::vec3& v2Norm = glm::normalize(v2);

		// Get the angle theta between v1 and v2, and its cosine(v1v2Dot)
		float v1v2Dot = glm::clamp(glm::dot(v1Norm, v2Norm), -1.0f, 1.0f);
		float theta = acos(v1v2Dot);

		// Get the axis around which the current joint will rotate by theta
		glm::vec3 axis(0.0f, 0.0f, 1.0f);
		if (glm::abs(v1v2Dot) < 1.0f)
			axis = glm::normalize(glm::cross(v1Norm, v2Norm));


		// Build a quaternion from the axis angle rotation we just computed
		// This rotation is applied to the joint's local orientation
		glm::quat rot = glm::angleAxis(theta, axis);
		currNode->m_localTr.m_orientation = rot * currNode->m_localTr.m_orientation;
	}


	// Update the world transforms of all the nodes starting from the given one, until the end effector
	void CCD3DSolver::update_world_transforms(SceneNode* start, SceneNode* endEffector)
	{
		// Store all the nodes that we need to update (do so from end effector until start)
		std::list<SceneNode*> nodes;

		// Any siblings that are not part of the ik chain will be updated with the regular scene update

		SceneNode* traverser = endEffector;
		while (traverser != nullptr)
		{
			nodes.push_front(traverser);

			if (traverser == start)
				break;

			traverser = traverser->get_parent();
		}

		// Update each node from start until end effector
		for (SceneNode* currNode : nodes)
		{
			if (currNode->get_parent())
				currNode->m_worldTr.concatenate(currNode->m_localTr, currNode->get_parent()->m_worldTr);
			else
				currNode->m_worldTr = currNode->m_localTr;

			//currNode->m_updateWorldTr = false;
		}
	}


	// Check whether a solution has been found
	bool CCD3DSolver::check_solution(const glm::vec3& endEffectorWorldPos, const glm::vec3& targetWorldPos)
	{
		float distToTarget = glm::length(targetWorldPos - endEffectorWorldPos);
		if (distToTarget < m_solutionTolerance)
			return true;

		return false;
	}
}