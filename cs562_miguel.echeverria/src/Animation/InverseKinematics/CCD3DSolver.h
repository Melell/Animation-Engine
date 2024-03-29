/**
* @file CCD3DSolver.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Solves ik problems in 3d using the Cyclic Coordinate Descent method.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IKSolver.h"


namespace cs460
{
	class SceneNode;


	class CCD3DSolver : public IKSolver
	{
	public:

		CCD3DSolver();
		virtual ~CCD3DSolver();

		IKSolverStatus solve() override;	// Tries to solve the internal IK chain
		void reset_solver() override;		// Resets all of this solver's data

		float m_solutionTolerance = 0.1f;
		unsigned m_maxIterations = 1;

		void on_gui() override;

	private:

		// Main part of the CCD algorithm, uses the end effector and target's world positions to compute the local rotation of currNode
		void apply_local_rotation(SceneNode* currNode, const glm::vec3& endWorldPos, const glm::vec3& targetWorldPos);

		// Update the world transforms of all the nodes starting from the given one, until the end effector
		void update_world_transforms(SceneNode* start, SceneNode* endEffector);

		// Check whether a solution has been found
		bool check_solution(const glm::vec3& endEffectorWorldPos, const glm::vec3& targetWorldPos);
	};
}