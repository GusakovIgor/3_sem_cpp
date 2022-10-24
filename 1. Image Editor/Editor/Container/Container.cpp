#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include "Container.hpp"

#include <algorithm>
#include <functional>
#include <cassert>
#include <cstring>
#include <cmath>


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
		command->SetPrevImage (image);

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

		if (command->GetPrevImage ())
		{
			delete image;

			image = command->GetPrevImage ();
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


bool Container::ImproveClarity (Command* improve_clarity_base)
{
	vector <vector <float>> filter_matrix (3, vector <float> (3, -1.0));

	filter_matrix[1][1] = 9.0;

	image->ApplyFilterMatrix (filter_matrix);

    return true;
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


bool Container::GreyFilter (Command* grey_filter_base)
{
	for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
        	uint32_t average_component = (image->pixels[x][y].r +
        								  image->pixels[x][y].g +
        								  image->pixels[x][y].b) / 3;

			image->pixels[x][y] = Pixel (average_component, average_component, average_component);
        }
    }

    return true;
}


bool Container::EdgeDetection (Command* edge_detection_base)
{
	EdgeDetectionCmd* edge_detection = dynamic_cast <EdgeDetectionCmd*> (edge_detection_base);

	uint32_t detection_border = 255 - 255 * edge_detection->DetectionPercent () / 100;


	GreyFilter (nullptr);

	Image* temp_image = new Image ();
	*temp_image = *image;

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

	temp_image->ApplyFilterMatrix (second_sobel_matrix);

	for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
			uint32_t new_r = std::min (255u, static_cast <uint32_t> (sqrt (image->pixels[x][y].r * image->pixels[x][y].r + temp_image->pixels[x][y].r * temp_image->pixels[x][y].r)));
			uint32_t new_g = std::min (255u, static_cast <uint32_t> (sqrt (image->pixels[x][y].g * image->pixels[x][y].g + temp_image->pixels[x][y].g * temp_image->pixels[x][y].g)));
			uint32_t new_b = std::min (255u, static_cast <uint32_t> (sqrt (image->pixels[x][y].b * image->pixels[x][y].b + temp_image->pixels[x][y].b * temp_image->pixels[x][y].b)));

			if (new_r <= detection_border ||
				new_g <= detection_border ||
				new_b <= detection_border)
			{
				new_r = new_g = new_b = 0;
			}

			image->pixels[x][y] = Pixel (new_r, new_g, new_b);
        }
    }

    delete temp_image;

    return true;
}


bool Container::ReduceNoise (Command* reduce_noise_base)
{
	ReduceNoiseCmd* reduce_noise = dynamic_cast <ReduceNoiseCmd*> (reduce_noise_base);

	int reduce_matrix_size = 10 * reduce_noise->ReducePercent () / 100;

	if (reduce_matrix_size == 0)
	{
	    return true;
	}

	vector <vector <Pixel>> temp_pixels (image->width, vector <Pixel> (image->height));

    int start_offset = - (reduce_matrix_size - 1) / 2;
    int end_offset   =   (reduce_matrix_size - 1) / 2;

    for (int x = 0; x < image->width; ++x)
    {
        for (int y = 0; y < image->height; ++y)
        {
			vector <uint8_t> surroundings_r;
			vector <uint8_t> surroundings_g;
			vector <uint8_t> surroundings_b;

            for (int i = start_offset; i <= end_offset; ++i)
            {
                for (int j = start_offset; j <= end_offset; ++j)
                {
                	if (x + i < 0 ||
                		y + j < 0 ||
                		x + i >= image->width ||
                		y + j >= image->height)
                	{
                		continue;
                	}

                    surroundings_r.push_back (image->pixels[x + i][y + j].r);
                    surroundings_g.push_back (image->pixels[x + i][y + j].g);
                    surroundings_b.push_back (image->pixels[x + i][y + j].b);
                }
            }

            std::sort (surroundings_r.begin (), surroundings_r.end ());
            std::sort (surroundings_g.begin (), surroundings_g.end ());
            std::sort (surroundings_b.begin (), surroundings_b.end ());

            uint32_t median_index = (surroundings_r.size () - 1) / 2;

            temp_pixels[x][y] = Pixel (surroundings_r[median_index], surroundings_g[median_index], surroundings_b[median_index]);
    	}
    }

    for (int x = 0; x < image->width; ++x)
    {
        for (int y = 0; y < image->height; ++y)
        {
        	image->pixels[x][y] = temp_pixels[x][y];
        }
    }

    return true;
}


bool Container::Vignette (Command* vignette_base)
{


    return true;
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

	image = new_image;

    return true;
}


bool Container::Compress (Command* compress_base)
{


    return true;
}