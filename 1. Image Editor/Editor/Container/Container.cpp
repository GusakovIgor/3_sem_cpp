#include <Image.hpp>
#include "Container.hpp"


Container::Container (const string& init_name)
{
	name = init_name;

	image = nullptr;
}

Container::~Container ()
{
	if (image)
	{
		delete image;
	}
}


void Container::SetImage (const string& new_image_name, Image* new_image)
{
	image = new_image;

	image_name = new_image_name;
}

Image* Container::GetImage ()
{
	return image;
}


string Container::Name ()
{
	return name;
}

string Container::ImageName ()
{
	return image_name;
}