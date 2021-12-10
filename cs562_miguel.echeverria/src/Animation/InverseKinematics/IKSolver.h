/**
* @file IKSolver.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Base class for all the specific ik problem solving methods.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	struct IKChain;


	enum class IKSolverStatus
	{
		IDLE,
		PROCESSING,
		SUCCESS,
		FAILURE
	};

	enum class IKSolverType
	{
		ANALYTIC_2BONE_2D,
		CCD_3D,
		FABRIK_3D
	};


	class IKSolver
	{
	public:

		IKSolver();
		virtual ~IKSolver();

		virtual IKSolverStatus solve() = 0;		// Performs an iteration of the solve loop (if any)
		virtual void reset_solver() = 0;		// Reset all the solver data


		// Set the IK chain that this instance will try to solve. If the
		// same as the current one, ignore, otherwise, the solver's data
		// will be reset and status will be IDLE again.
		void set_ik_chain(IKChain* chain);

		// Query the current status of the solver
		IKSolverStatus get_status() const;

	protected:
		IKChain* m_chain = nullptr;
		IKSolverStatus m_status = IKSolverStatus::IDLE;
	};
}