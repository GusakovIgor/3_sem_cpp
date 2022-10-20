#include <Image.hpp>
#include <Container.hpp>
#include <Commands.hpp>
#include <CommandFactory.hpp>
#include <InputManager.hpp>
#include <BmpImage.hpp>
#include <Loader.hpp>
#include <Saver.hpp>
#include <ContainerManager.hpp>
#include "Editor.hpp"


Editor::Editor ()
{
	input_manager     = new InputManager (&current_command);
	container_manager = new ContainerManager ();
}

Editor::Editor (const int argc, const char* argv[])
{
	if (argc == 1)
	{
		input_manager = new InputManager (&current_command);
	}
	else
	{
		input_manager = new InputManager (&current_command, argc, argv);
	}

	container_manager = new ContainerManager ();

}

Editor::~Editor ()
{
	delete input_manager;
	delete container_manager;
}

void Editor::operator() ()
{
	while (running)
	{
		input_manager->ParseCommand ();

		if (current_command && current_command->Type () == CommandType::cmd_exit)
		{
			running = false;

			delete current_command;
			current_command = nullptr;
		}

		if (current_command)
		{
			container_manager->Execute (&current_command);
			// algorithm_manager->Execute (&current_command);
		}
	}
}