#include "Image.hpp"



Pixel::Pixel ()
{
	r = 0;
	g = 0;
	b = 0;
}

Pixel::Pixel (const uint8_t init_r, const uint8_t init_g, const uint8_t init_b)
{
	r = init_r;
	g = init_g;
	b = init_b;
}

ifstream& operator >> (ifstream& input, Pixel& pixel)
{
    input.read ((char*) (&pixel.r), sizeof (pixel.r));
    input.read ((char*) (&pixel.g), sizeof (pixel.g));
    input.read ((char*) (&pixel.b), sizeof (pixel.b));

    return input;
}

ofstream& operator << (ofstream& output, const Pixel& pixel)
{
    output.write ((char*) (&pixel.r), sizeof (pixel.r));
    output.write ((char*) (&pixel.g), sizeof (pixel.g));
    output.write ((char*) (&pixel.b), sizeof (pixel.b));

    return output;
}



Image::Image ()
{
	width  = 0;
	height = 0;
}

Image::Image (const int32_t init_width, const int32_t init_height)
{
	width  = init_width;
	height = init_height;

	pixels.resize (width, vector <Pixel> (height));
}

int32_t Image::Size () const
{
	return width * height * sizeof (Pixel);
}

ifstream& operator >> (ifstream& input, Image& image)
{
    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
        {
            input >> image.pixels[x][y];
        }
    }

    return input;
}

ofstream& operator << (ofstream& output, const Image& image)
{
    for (uint32_t y = 0; y < image.height; ++y)
    {
        for (uint32_t x = 0; x < image.width; ++x)
        {
            output << image.pixels[x][y];
        }
    }

    return output;
}