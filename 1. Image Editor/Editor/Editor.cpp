#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include <CommandFactory.hpp>
#include <InputManager.hpp>
#include <Container.hpp>
#include <Loader.hpp>
#include <Saver.hpp>
#include <ContainerManager.hpp>
#include "Editor.hpp"

#include <filesystem>


Editor::Editor ()
{
	input_manager     = new InputManager ();
	container_manager = new ContainerManager ();
}

Editor::Editor (const int argc, const char* argv[])
{
	if (argc == 1)
	{
		input_manager = new InputManager ();
	}
	else
	{
		input_manager = new InputManager (argc, argv);
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
		Command* new_command = input_manager->ParseCommand ();

		if (new_command)
		{
			Execute (new_command);
		}
		else
		{
			running = false;
		}
	}
}


void Editor::Execute (Command* command)
{
	if (operations.contains (command->Type ()))
	{
		std::invoke (operations.at (command->Type ()), *this, command);

		delete command;
	}
	else
	{
		container_manager->Execute (command);
	}
}


void Editor::ListDirectory (Command* ls_base)
{
	for (const auto& dentry : std::filesystem::directory_iterator (std::filesystem::current_path ()))
	{
		std::cout << (dentry.path ()).filename () << std::endl;
	}

	std::cout << std::endl;
}

void Editor::ChangeDirectory (Command* cd_base)
{
	ChangeDirectoryCmd* cd = dynamic_cast <ChangeDirectoryCmd*> (cd_base);

	std::filesystem::current_path (cd->TargetDirectory ());
}

void Editor::Exit (Command* exit_base)
{
	running = false;
}

void Editor::Help (Command* help_base)
{
	std::cout << "You found help command. You're doing fine!" << std::endl;
	std::cout << "Go on and find out more ways to use this program" << std::endl << std::endl;
}