#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include "Container.hpp"

#include <functional>
#include <cassert>
#include <cstring>


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

	while (!applied_commands.empty ())
	{
		Command* command = applied_commands.front ();
		applied_commands.pop ();

		delete command;
	}
}



void Container::Execute (Command* command)
{
	if (operations.contains (command->Type ()))
	{
		if (std::invoke (operations.at (command->Type ()), *this, command))
		{
			applied_commands.push (command);
		}
	}
	else
	{
		std::cout << "Implementing of this command is currently in progress" << std::endl;
	}
}

void Container::Cancel ()
{
	if (!applied_commands.empty ())
	{
		Command* command = applied_commands.front ();
		applied_commands.pop ();

		if (rev_operations.contains (command->Type ()))
		{
			std::invoke (rev_operations.at (command->Type ()), *this, command);

			delete command;
		}
		else
		{
			std::cout << "Implementing of reverse to last used command is currently in progress" << std::endl;
		}
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



bool Container::Negative (Command* negative_base)
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

    return true;
}

void Container::RevNegative (Command* negative_base)
{
	Negative (negative_base);
}


bool Container::ReplaceColor (Command* replace_color_base)
{
	ReplaceColorCmd* replace_colour = dynamic_cast <ReplaceColorCmd*> (replace_color_base);

	for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
        	if (image->pixels[x][y] == replace_colour->TargetColour ())
        	{
        		image->pixels[x][y] = replace_colour->NewColour ();
        	}
        }
    }

    return true;
}

void Container::RevReplaceColor (Command* replace_color_base)
{
	ReplaceColorCmd* replace_colour = dynamic_cast <ReplaceColorCmd*> (replace_color_base);

	for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
        	if (image->pixels[x][y] == replace_colour->NewColour ())
        	{
        		image->pixels[x][y] = replace_colour->TargetColour ();
        	}
        }
    }
}


bool Container::ImproveClarity (Command* improve_clarity_base)
{
	vector <vector <float>> filter_matrix (3, vector <float> (3, -1.0));

	filter_matrix[1][1] = 9.0;

	image->ApplyFilterMatrix (filter_matrix);

    return true;
}

void Container::RevImproveClarity (Command* improve_clarity_base)
{

}


bool Container::GaussianFilter (Command* gaussian_filter_base)
{
	vector <vector <float>> filter_matrix =
	{
		{0.054901, 0.111345, 0.054901},
		{0.111345, 0.225821, 0.111345},
		{0.054901, 0.111345, 0.054901}
	};

	image->ApplyFilterMatrix (filter_matrix);

    return true;
}

void Container::RevGaussianFilter (Command* gaussian_filter_base)
{

}


bool Container::GreyFilter (Command* grey_filter_base)
{
	for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
        	uint32_t average_colour = 0;
        	average_colour += image->pixels[x][y].r;
        	average_colour += image->pixels[x][y].g;
        	average_colour += image->pixels[x][y].b;
        	average_colour /= 3;
			image->pixels[x][y] = Pixel (average_colour, average_colour, average_colour);
        }
    }

    return true;
}

void Container::RevGreyFilter (Command* grey_filter_base)
{

}


bool Container::EdgeDetection (Command* edge_detection_base)
{
	vector <vector <float>> first_sobel_matrix =
	{
		{-1.0, -2.0, -1.0},
		{ 0.0,  0.0,  0.0},
		{ 1.0,  2.0,  1.0}
	};

	image->ApplyFilterMatrix (first_sobel_matrix);

	vector <vector <float>> second_sobel_matrix =
	{
		{-1.0, 0.0, 1.0},
		{-2.0, 0.0, 2.0},
		{-1.0, 0.0, 1.0}
	};

	image->ApplyFilterMatrix (second_sobel_matrix);

    return true;
}

void Container::RevEdgeDetection (Command* edge_detection_base)
{

}


bool Container::ReduceNoise (Command* reduce_noise_base)
{


    return true;
}

void Container::RevReduceNoise (Command* reduce_noise_base)
{

}


bool Container::Vignette (Command* vignette_base)
{


    return true;
}

void Container::RevVignette (Command* vignette_base)
{

}


bool Container::Crop (Command* crop_base)
{
	CropCmd* crop = dynamic_cast <CropCmd*> (crop_base);

	if (crop->StartX ()    < 0 ||
		crop->StartY ()    < 0 ||
		crop->NewWidth ()  < 0 ||
		crop->NewHeight () < 0)
	{
		std::cout << "Seriously? Negative crop?.. Think of something better to test with." << std::endl << std::endl;

	    return false;
	}

	if (crop->StartX () + crop->NewWidth () > image->width)
	{
		std::cout << "Wrong crop: current image width is " << image->width << std::endl << std::endl;

	    return false;
	}

	if (crop->StartY () + crop->NewHeight () > image->height)
	{
		std::cout << "Wrong crop: current image height is " << image->height << std::endl << std::endl;

	    return false;
	}

	Image* new_image = new Image (crop->NewWidth (), crop->NewHeight ());
	new_image->header = image->header;
	new_image->dib_header = image->dib_header;
	new_image->colour_header = image->colour_header;

	for (int x = 0; x < crop->NewWidth (); ++x)
	{
		for (int y = 0; y < crop->NewHeight (); ++y)
		{
			new_image->pixels[x][y] = image->pixels[crop->StartX () + x][crop->StartY () + y];
		}
	}

	crop->SetOldImage (image);

	image = new_image;

    return true;
}

void Container::RevCrop (Command* crop_base)
{
	CropCmd* crop = dynamic_cast <CropCmd*> (crop_base);

	delete image;

	image = crop->OldImage ();
}


bool Container::Compress (Command* compress_base)
{


    return true;
}

void Container::RevCompress (Command* compress_base)
{

}
