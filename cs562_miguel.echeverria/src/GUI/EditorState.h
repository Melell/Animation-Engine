/**
* @file EditorState.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief The EditorState class contains all editor related data.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class SceneNode;


	struct EditorState
	{
		SceneNode* m_selectedNode;
		char m_textBuffer[128] = { '\0' };
	};
}