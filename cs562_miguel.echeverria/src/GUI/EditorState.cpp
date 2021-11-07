#include "pch.h"
#include "EditorState.h"
#include "Editor.h"



namespace cs460
{
	// Use this to avoid having to include the entire editor just to get the editor state
	EditorState& EditorState::get_main_editor_state()
	{
		return Editor::get_instance().get_state();
	}
}