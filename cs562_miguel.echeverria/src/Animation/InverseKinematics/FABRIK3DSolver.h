#pragma once

#include "IKSolver.h"


namespace cs460
{
	class FABRIK3DSolver : public IKSolver
	{
	public:

		FABRIK3DSolver();
		virtual ~FABRIK3DSolver();

		IKSolverStatus solve() override;	// Tries to solve the internal IK chain
		void reset_solver() override;		// Resets all of this solver's data
	};
}