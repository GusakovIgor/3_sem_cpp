#include <Image.hpp>
#include <Container.hpp>
#include <Commands.hpp>
#include <CommandFactory.hpp>
#include "InputManager.hpp"


InputManager::InputManager (Command** command)
{
	current_command = command;

	initial_input = std::cin.rdbuf ();
}

InputManager::InputManager (Command** command, const int argc, const char* argv[])
{
	current_command = command;

	initial_input = std::cin.rdbuf ();

	string input_base;

	for (int32_t i = 1; i < argc; ++i)
	{
		string new_token (argv[i]);

		if (options_to_commands.contains (new_token))
		{
			new_token = options_to_commands.at (new_token);
		}

		input_base += new_token + " ";
	}

	std::cin.rdbuf (std::stringstream (input_base).rdbuf ());
}

InputManager::~InputManager ()
{
	std::cin.rdbuf (initial_input);
}

bool InputManager::ParseCommand ()
{
	if (std::cin.eof ())
	{
		return false;
	}

	string cmd_name;
	std::cin >> cmd_name;

	*current_command = command_factory->New (cmd_name);

	if (*current_command == nullptr)
	{
		std::cout << "Unknowkn command: \"" << cmd_name << "\"" << std::endl;

		return false;
	}

	std::cin >> **current_command;

	return true;
}