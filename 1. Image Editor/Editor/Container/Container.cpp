#include <Image.hpp>
#include <Commands.hpp>
#include "Container.hpp"

#include <functional>


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



void Container::Execute (Command* command)
{
	if (operations.contains (command->Type ()))
	{
		std::invoke (operations.at (command->Type ()), *this, command);

		delete command;
	}
	else
	{
		std::cout << "Implementing of this command is currently in progress" << std::endl;
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


void Container::Negative (Command* negative_base)
{
    for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
            image->pixels[x][y].r = 255 - image->pixels[x][y].r;
            image->pixels[x][y].g = 255 - image->pixels[x][y].g;
            image->pixels[x][y].b = 255 - image->pixels[x][y].b;
        }
    }
}

void Container::ReplaceColor (Command* replace_color_base)
{

}

void Container::ImproveClarity (Command* improve_clarity_base)
{

}

void Container::GaussianFilter (Command* gaussian_filter_base)
{

}

void Container::GreyFilter (Command* grey_filter_base)
{

}

void Container::EdgeDetection (Command* edge_detection_base)
{

}

void Container::ReduceNoise (Command* reduce_noise_base)
{

}

void Container::Vignette (Command* vignette_base)
{

}

void Container::Crop (Command* crop_base)
{

}

void Container::Compress (Command* compress_base)
{

}