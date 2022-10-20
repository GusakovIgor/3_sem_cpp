#ifndef EDITOR_HPP
#define EDITOR_HPP

class Editor
{

public:

	Editor ();
	Editor (const int argc, const char* argv[]);
	~Editor ();

	void operator() ();

private:

	bool running;
	bool interactive;

	Command* current_command;

	InputManager*     input_manager;
	ContainerManager* container_manager;
};


#endif // EDITOR_HPP