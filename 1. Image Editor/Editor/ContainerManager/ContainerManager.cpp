#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include <Container.hpp>
#include <Loader.hpp>
#include <Saver.hpp>
#include "ContainerManager.hpp"

#include <functional>
#include <iomanip>

ContainerManager::ContainerManager ()
{
	active_container = 0;
	containers.push_back (new Container ("default"));

	loader = new Loader ();
	saver  = new Saver  ();
}

ContainerManager::~ContainerManager ()
{
	for (int32_t i = 0; i < containers.size (); ++i)
	{
		delete containers[i];
	}

	delete loader;
	delete saver;
}

void ContainerManager::Execute (Command* command)
{
	if (operations.contains (command->Type ()))
	{
		std::invoke (operations.at (command->Type ()), *this, command);

		delete command;
	}
	else
	{
		if (command->Type () != CommandType::cmd_cancel)
		{
			containers[active_container]->Execute (command);
		}
		else
		{
			containers[active_container]->Cancel ();
		}
	}
}


void ContainerManager::LoadImageToContainer (Command* load_image_base)
{
	LoadCmd* load_image = dynamic_cast <LoadCmd*> (load_image_base);

	Image* new_image = loader->LoadImage (load_image->Name ());

	containers[active_container]->SetImage (load_image->Name (), new_image);
}

void ContainerManager::SaveImageFromContainer (Command* save_image_base)
{
	SaveCmd* save_image = dynamic_cast <SaveCmd*> (save_image_base);

	Image* old_image = containers[active_container]->GetImage ();

	saver->SaveImage (save_image->Name (), old_image);
}


void ContainerManager::AddContainer (Command* add_container_base)
{
	AddContainerCmd* add_container = dynamic_cast <AddContainerCmd*> (add_container_base);
	containers.push_back (new Container (add_container->Name ()));

	active_container = containers.size () - 1;
}

void ContainerManager::DelContainer (Command* del_container_base)
{
	DelContainerCmd* del_container = dynamic_cast <DelContainerCmd*> (del_container_base);

	delete containers[del_container->TargetContainer () - 1];
	containers.erase (containers.begin () + del_container->TargetContainer () - 1);

	active_container = containers.size () - 1;
}

void ContainerManager::ListContainers (Command* list_containers_base)
{
	if (containers.size () > 0)
	{
		std::cout << "Containers:" << std::endl;
		for (int32_t i = 0; i < containers.size (); ++i)
		{
			std::cout << i + 1 << ". " << containers[i]->Name () << ": " << "holds " << std::setw (10);

			if (containers[i]->GetImage ())
			{
				std::cout << containers[i]->ImageName ();
			}
			else
			{
				std::cout << "no image";
			}

			if (i == active_container)
			{
				std::cout << " *";
			}

			std::cout << std::endl;
		}
	}
	else
	{
		std::cout << "No containers" << std::endl;
	}
}

void ContainerManager::SwitchContainer (Command* swicth_container_base)
{
	SwitchContainerCmd* switch_container = dynamic_cast <SwitchContainerCmd*> (swicth_container_base);

	active_container = switch_container->TargetContainer () - 1;
}