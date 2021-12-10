#include "pch.h"
#include "FABRIK3DSolver.h"


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
		return IKSolverStatus::IDLE;
	}

	// Resets all of this solver's data
	void FABRIK3DSolver::reset_solver()
	{
		m_status = IKSolverStatus::IDLE;
	}
}