#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include <CommandFactory.hpp>
#include "InputManager.hpp"


InputManager::InputManager ()
{
	initial_input = std::cin.rdbuf ();
}

InputManager::InputManager (const int argc, const char* argv[])
{
	initial_input = std::cin.rdbuf ();

	string input_base;

	for (int32_t i = 1; i < argc; ++i)
	{
		string new_token (argv[i]);

		if (options_to_commands.contains (new_token))
		{
			new_token = options_to_commands.at (new_token);
		}

		input_base += new_token;

		if (i != argc - 1)
		{
			input_base += " ";
		}
	}

	actual_input = std::stringstream (input_base);

	std::cin.rdbuf (actual_input.rdbuf ());
}

InputManager::~InputManager ()
{
	std::cin.rdbuf (initial_input);
}

Command* InputManager::ParseCommand ()
{
	if (std::cin.eof ())
	{
		return command_factory->New ("exit");
	}

	string cmd_name;
	std::cin >> cmd_name;

	Command* new_command = command_factory->New (cmd_name);

	if (!new_command)
	{
		std::cout << "Unknowkn command: \"" << cmd_name << "\"" << std::endl;

		return nullptr;
	}

	std::cin >> *new_command;

	return new_command;
}