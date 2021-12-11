#pragma once

#include "IKSolver.h"


namespace cs460
{
	class SceneNode;


	class FABRIK3DSolver : public IKSolver
	{
	public:

		FABRIK3DSolver();
		virtual ~FABRIK3DSolver();

		IKSolverStatus solve() override;	// Tries to solve the internal IK chain
		void reset_solver() override;		// Resets all of this solver's data

		float m_solutionTolerance = 0.1f;
		unsigned m_maxIterations = 1;

	private:

		void store_world_positions(SceneNode* chainRoot, SceneNode* endEffector, std::vector<glm::vec3>& jointsWorldPos);
		void store_bone_lengths(SceneNode* chainRoot, SceneNode* endEffector, std::vector<float>& boneLengths);

		void forward_step(std::vector<glm::vec3>& worldPositions, const glm::vec3& targetWorldPos, const std::vector<float>& boneLengths);

		void backward_step(std::vector<glm::vec3>& worldPositions, const std::vector<float>& boneLengths, const glm::vec3& originalRootWorldPos);

		// Store the ik chain hierarchy of nodes in the given list
		void store_chain_nodes(SceneNode* start, SceneNode* end, std::list<SceneNode*>& nodes);

		bool check_solution(const glm::vec3& endEffectorWorldPos, const glm::vec3& targetWorldPos);

		void update_local_rotations(SceneNode* chainRoot, SceneNode* endEffector, const std::vector<glm::vec3>& solutionWorldPos);

		void update_chain_world(SceneNode* start, SceneNode* endEffector);
	};
}