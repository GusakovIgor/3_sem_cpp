#ifndef EDITOR_HPP
#define EDITOR_HPP


class Editor;

typedef void (Editor::*EditorCmdPtr) (Command* command);

class Editor
{

public:

    Editor ();
    Editor (const int argc, const char* argv[]);
    ~Editor ();

    void operator() ();

    void Execute (Command* command);

private:

    bool running;

    InputManager*     input_manager;
    ContainerManager* container_manager;

    void ListDirectory   (Command* ls_base);
    void ChangeDirectory (Command* cd_base);
    void Exit            (Command* exit_base);
    void Help            (Command* help_base);

    const unordered_map <CommandType, EditorCmdPtr> operations =
    {
        {CommandType::cmd_ls,   &Editor::ListDirectory},
        {CommandType::cmd_cd,   &Editor::ChangeDirectory},
        {CommandType::cmd_exit, &Editor::Exit},
        {CommandType::cmd_help, &Editor::Help}
    };
};


#endif // EDITOR_HPP