#ifndef EDITOR_HPP
#define EDITOR_HPP

class Editor;

typedef void (Editor::*EditorCommandPtr) (Command* command);

class Editor
{

public:

	Editor ();
	Editor (const int argc, const char* argv[]);
	~Editor ();

	void operator() ();

	void Execute (Command* command);

	void ListDirectory   (Command* ls_base);
	void ChangeDirectory (Command* cd_base);
	void Exit 			 (Command* exit_base);
	void Help 			 (Command* help_base);


private:

	bool running;

	InputManager*     input_manager;
	ContainerManager* container_manager;

	const unordered_map <CommandType, EditorCommandPtr> operations =
	{
		{CommandType::cmd_ls,   &Editor::ListDirectory},
		{CommandType::cmd_cd,   &Editor::ChangeDirectory},
		{CommandType::cmd_exit, &Editor::Exit},
		{CommandType::cmd_help, &Editor::Help}
	};
};


#endif // EDITOR_HPP