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
#include <istream>


const char ESC_CSI[] = "\x1b[";


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
    Clear (nullptr);

    while (running)
    {
        std::cout << ESC_CSI << "38;2;" << "0;204;153" << "m";
        std::cout << container_manager->ActiveContainerName () << ':';
        std::cout << ESC_CSI << "38;2;" << "0;102;255" << "m";
        std::cout << std::filesystem::current_path () << "$ ";
        std::cout << ESC_CSI << "0" << "m";
        Command* new_command = input_manager->ParseCommand ();

        if (new_command)
        {
            Execute (new_command);
        }
        else
        {
            std::cout << "Failed to read next command" << std::endl;
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
}

void Editor::ChangeDirectory (Command* cd_base)
{
    ChangeDirectoryCmd* cd = dynamic_cast <ChangeDirectoryCmd*> (cd_base);

    std::filesystem::current_path (cd->TargetDirectory ());
}

void Editor::Exit (Command* exit_base)
{
    Clear (nullptr);

    running = false;
}

void Editor::Clear (Command* clear_base)
{
    std::cout << ESC_CSI << "2J";
    std::cout << ESC_CSI << "1H";
}

void Editor::Help (Command* help_base)
{
    std::ifstream help_file ("Help.txt");

    if (help_file.is_open ())
    {
        std::cout << help_file.rdbuf () << std::endl;
    }
    else
    {
        std::cout << "No help file found. Using older help version:" << std::endl;
        std::cout << "You found help command. You're doing fine!" << std::endl;
        std::cout << "Go on and find out more ways to use this program" << std::endl << std::endl;
    }
}