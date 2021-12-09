/**
* @file IKChainRoot.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/04/12
* @brief Component that represents an ik chain (only added to the root of the chain).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"
#include "Animation/InverseKinematics/IKSolver.h"


namespace cs460
{
	struct IKChain;
	class IKSolver;


	class IKChainRoot : public IComponent
	{
	public:

		IKChainRoot();
		virtual ~IKChainRoot();

		void initialize();
		IKSolverStatus update();
		void debug_draw(const glm::vec4& boneColor, const glm::vec4& boneHighlightColor, const glm::vec4& jointColor, const glm::vec4& endEffectorColor);

		void set_end_effector(SceneNode* endEffector);

		IKChain* get_chain() const;
		IKSolver* get_solver() const;

	private:
		IKChain* m_chain = nullptr;
		IKSolver* m_solver = nullptr;
		glm::vec2 m_lastEndEffectorLocalPos{0.0f, 0.0f};
		bool m_drawChain = true;

		virtual void on_gui();	// To be overriden by specific components
	};
}