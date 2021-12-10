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

		unsigned m_maxIterations = 4;

	private:

		// Main part of the CCD algorithm, uses the end effector and target's world positions to compute the local rotation of currNode
		void set_local_rotation(SceneNode* currNode, const glm::vec3& endWorldPos, const glm::vec3& targetWorldPos);
	};
}