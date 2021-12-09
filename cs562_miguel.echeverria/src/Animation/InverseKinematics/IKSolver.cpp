/**
* @file IKSolver.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Base class for all the specific ik problem solving methods.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "IKSolver.h"
#include "IKChain.h"


namespace cs460
{
	IKSolver::IKSolver()
	{
	}

	IKSolver::~IKSolver()
	{
	}


	// Performs an iteration of the solve loop (if any)
	IKSolverStatus IKSolver::solve()
	{
	}

	// Reset all the solver data
	void IKSolver::reset_solver()
	{
	}


	// Set the IK chain that this instance will try to solve. If the
	// same as the current one, ignore, otherwise, the solver's data
	// will be reset and status will be IDLE again.
	void IKSolver::set_ik_chain(IKChain* chain)
	{
		if (m_chain == chain)
			return;

		m_chain = chain;
		m_status = IKSolverStatus::IDLE;
		reset_solver();
	}


	IKSolverStatus IKSolver::get_status() const
	{
		return m_status;
	}
}