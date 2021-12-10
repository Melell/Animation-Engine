/**
* @file Analytic2Bone2DSolver.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Solves ik problems in 2d involving only 3 joints (2 bones).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "IKSolver.h"


namespace cs460
{
	class Analytic2Bone2DSolver : public IKSolver
	{
	public:

		Analytic2Bone2DSolver();
		virtual ~Analytic2Bone2DSolver();

		IKSolverStatus solve() override;	// Tries to solve the internal IK chain
		void reset_solver() override;		// Resets all of this solver's data

	private:

	};
}