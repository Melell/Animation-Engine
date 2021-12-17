/**
* @file FABRIK3DSolver.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Solves ik problems in 3d using the Forwards And Backwards Reaching IK method.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "FABRIK3DSolver.h"
#include "Composition/SceneNode.h"
#include "Animation/InverseKinematics/IKChain.h"
#include <GLFW/glfw3.h>


namespace cs460
{
	FABRIK3DSolver::FABRIK3DSolver()
	{
	}

	FABRIK3DSolver::~FABRIK3DSolver()
	{
	}


	// Tries to solve the internal IK chain
	IKSolverStatus FABRIK3DSolver::solve()
	{
		m_status = IKSolverStatus::PROCESSING;

		SceneNode* chainRoot = m_chain->get_chain_root();
		SceneNode* endEffector = m_chain->get_end_effector();
		SceneNode* target = m_chain->get_target();
		if (chainRoot == nullptr || endEffector == nullptr || target == nullptr)
		{
			std::cout << "ERROR : FABRIK 3D SOLVER : Either chain root, end effector or target are not present!!!\n";
			m_status = IKSolverStatus::FAILURE;
			return m_status;
		}


		// Get the target world position and the original chain root world position
		const glm::vec3& targetWorldPos = target->m_worldTr.m_position;
		glm::vec3 originalRootWorldPos = chainRoot->m_worldTr.m_position;

		// Store the all the bone lengths as well as the original joints world positions
		std::vector<float> boneLengths;
		std::vector<glm::vec3> worldPositions;
		store_bone_lengths(chainRoot, endEffector, boneLengths);
		store_world_positions(chainRoot, endEffector, worldPositions);

		m_status = IKSolverStatus::FAILURE;

		// Process until a solution is found, or max iterations is reached
		for (unsigned i = 0; i < m_maxIterations; ++i)
		{
			forward_step(worldPositions, targetWorldPos, boneLengths);
			backward_step(worldPositions, boneLengths, originalRootWorldPos);

			if (check_solution(worldPositions[0], targetWorldPos))
				m_status = IKSolverStatus::SUCCESS;
		}

		update_local_rotations(chainRoot, endEffector, worldPositions);

		return m_status;
	}

	// Resets all of this solver's data
	void FABRIK3DSolver::reset_solver()
	{
		m_status = IKSolverStatus::IDLE;
	}


	void FABRIK3DSolver::on_gui()
	{
		ImGui::SliderFloat("Solution Tolerance", &m_solutionTolerance, 0.01f, 0.5f, "%.2f");
		int maxIterations = m_maxIterations;
		if (ImGui::SliderInt("Max Iterations", &maxIterations, 1, 5, "%d"))
		{
			m_maxIterations = maxIterations < 1 ? 1 : maxIterations;
		}
	}


	void FABRIK3DSolver::store_world_positions(SceneNode* chainRoot, SceneNode* endEffector, std::vector<glm::vec3>& jointsWorldPos)
	{
		// Add in reverser order, start-end of vector is: end effector until chain root
		SceneNode* traverser = endEffector;
		while (traverser != nullptr && traverser != chainRoot->get_parent())
		{
			jointsWorldPos.push_back(traverser->m_worldTr.m_position);
			traverser = traverser->get_parent();
		}
	}

	void FABRIK3DSolver::store_bone_lengths(SceneNode* chainRoot, SceneNode* endEffector, std::vector<float>& boneLengths)
	{
		// Add in reverser order, start-end of vector is: end effector until chain root
		SceneNode* traverser = endEffector;
		while (traverser != nullptr && traverser != chainRoot)
		{
			SceneNode* parent = traverser->get_parent();
			glm::vec3 boneVec = traverser->m_worldTr.m_position - parent->m_worldTr.m_position;
			float boneLength = glm::length(boneVec);
			boneLengths.push_back(boneLength);
			traverser = traverser->get_parent();
		}
	}


	void FABRIK3DSolver::forward_step(std::vector<glm::vec3>& worldPositions, const glm::vec3& targetWorldPos, const std::vector<float>& boneLengths)
	{
		// Set the end effector to the target world position
		worldPositions[0] = targetWorldPos;

		// From the end effector until the chain root (not included)
		int limit = (int)worldPositions.size() - 1;
		for (int i = 0; i < limit; ++i)
		{
			glm::vec3 currWorldPos = worldPositions[i];
			glm::vec3 parentWorldPos = worldPositions[i + 1];

			float currBoneLength = boneLengths[i];
			glm::vec3 currBoneDir = parentWorldPos - currWorldPos;
			glm::vec3 currBoneDirNorm(-1.0f, 0.0f, 0.0f);
			if (glm::epsilonNotEqual(glm::length(currBoneDir), 0.0f, FLT_EPSILON))
				currBoneDirNorm = glm::normalize(currBoneDir);

			worldPositions[i + 1] = currWorldPos + currBoneDirNorm * currBoneLength;
		}
	}


	void FABRIK3DSolver::backward_step(std::vector<glm::vec3>& worldPositions, const std::vector<float>& boneLengths, const glm::vec3& originalRootWorldPos)
	{
		// Set the chain root to its original world position
		worldPositions[worldPositions.size() - 1] = originalRootWorldPos;

		// From the chain root until the end effector (not included)
		for (int i = worldPositions.size() - 1; i > 0; --i)
		{
			glm::vec3 currWorldPos = worldPositions[i];
			glm::vec3 childWorldPos = worldPositions[i - 1];

			glm::vec3 currBoneDir = childWorldPos - currWorldPos;
			glm::vec3 currBoneDirNorm(-1.0f, 0.0f, 0.0f);
			if (glm::epsilonNotEqual(glm::length(currBoneDir), 0.0f, FLT_EPSILON))
				currBoneDirNorm = glm::normalize(currBoneDir);

			worldPositions[i - 1] = currWorldPos + currBoneDirNorm * boneLengths[i - 1];
		}
	}


	// Store the ik chain hierarchy of nodes in the given list
	void FABRIK3DSolver::store_chain_nodes(SceneNode* start, SceneNode* end, std::list<SceneNode*>& nodes)
	{
		// Get the chain of nodes in the natural order: From chain root to end effector
		SceneNode* traverser = end;
		while (traverser != nullptr && traverser != start->get_parent())
		{
			nodes.push_front(traverser);
			traverser = traverser->get_parent();
		}
	}


	bool FABRIK3DSolver::check_solution(const glm::vec3& endEffectorWorldPos, const glm::vec3& targetWorldPos)
	{
		const glm::vec3& vec = targetWorldPos - endEffectorWorldPos;
		float distToTarget = glm::length(vec);

		if (distToTarget < m_solutionTolerance)
			return true;

		return false;
	}


	void FABRIK3DSolver::update_local_rotations(SceneNode* chainRoot, SceneNode* endEffector, const std::vector<glm::vec3>& solutionWorldPos)
	{
		// Get the chain of nodes in the natural order
		std::list<SceneNode*> chainNodes;
		store_chain_nodes(chainRoot, endEffector, chainNodes);

		// For each joint from the chain root until the end effector (not included)
		int i = (int)solutionWorldPos.size() - 1;
		auto endIt = std::prev(chainNodes.end());
		for (auto it = chainNodes.begin(); it != endIt; ++it, --i)
		{
			// Get the current joint and its children world positions
			auto childIt = std::next(it);
			const glm::vec3& worldPos = (*it)->m_worldTr.m_position;
			const glm::vec3& worldChildPos = (*childIt)->m_worldTr.m_position;


			// Get the axis angle rotation needed in world
			const glm::vec3& currWorldBone = glm::normalize(worldChildPos - worldPos);
			const glm::vec3& desiredWorldBone = glm::normalize(solutionWorldPos[i - 1] - solutionWorldPos[i]);

			float dot = glm::clamp(glm::dot(currWorldBone, desiredWorldBone), -1.0f, 1.0f);
			float angle = acos(dot);

			glm::vec3 axis(0.0f, 0.0f, 1.0f);
			if (glm::abs(dot) < 1.0f)
				axis = glm::normalize(glm::cross(currWorldBone, desiredWorldBone));

			// Apply the rotation in world space
			glm::quat rot = glm::angleAxis(angle, axis);
			(*it)->m_worldTr.m_orientation = rot * (*it)->m_worldTr.m_orientation;


			// Update the world transforms of the original chain joints
			update_chain_world(*childIt, endEffector);


			// Perform inverse concatenation of orientations to get the local orientation needed
			if ((*it)->get_parent())
			{
				glm::quat invParentRot = glm::inverse((*it)->get_parent()->m_worldTr.m_orientation);
				(*it)->m_localTr.m_orientation = invParentRot * (*it)->m_worldTr.m_orientation;
			}
			else
				(*it)->m_localTr.m_orientation = (*it)->m_worldTr.m_orientation;
		}
	}


	void FABRIK3DSolver::update_chain_world(SceneNode* start, SceneNode* endEffector)
	{
		// Store the nodes from start to endEffector (inclusive), in that order, in a list
		std::list<SceneNode*> nodes;
		store_chain_nodes(start, endEffector, nodes);

		// For each node in the hierarchy, update its world transform
		for (SceneNode* currNode : nodes)
			currNode->m_worldTr.concatenate(currNode->m_localTr, currNode->get_parent()->m_worldTr);
	}
}