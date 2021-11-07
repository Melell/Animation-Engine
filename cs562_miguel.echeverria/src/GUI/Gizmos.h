/**
* @file Gizmos.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Logic of the editor gizmos, using ImGuizmo library.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Gizmos
	{
	public:

		friend class Editor;

		void update();

	private:

		bool m_noOperation = false;

		Gizmos();
		Gizmos(const Gizmos&) = delete;
		Gizmos& operator=(const Gizmos&) = delete;
	};
}