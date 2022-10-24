#include <BmpImage.hpp>
#include "Image.hpp"

#include <iomanip>
#include <cstring>
#include <cassert>


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

istream& operator >> (istream& input, Pixel& pixel)
{
    int32_t colour = 0;
    input >> std::setbase (16) >> colour >> std::setbase (10);

    pixel.r = (colour >> 16) & 0xff;
    pixel.g = (colour >>  8) & 0xff;
    pixel.b = (colour      ) & 0xff;

    return input;
}

bool operator == (const Pixel& left, const Pixel& right)
{
    return (left.r == right.r &&
            left.g == right.g &&
            left.b == right.b);
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

Image::Image (const BmpHeader& init_header,
              const BmpInfoHeader& init_dib_header,
              const BmpColorHeader& init_colour_header) : Image (init_dib_header.width, init_dib_header.height)
{
    header = init_header;
    dib_header = init_dib_header;
    colour_header = init_colour_header;
}

int32_t Image::Size () const
{
	return width * height * sizeof (Pixel);
}

void Image::ApplyFilterMatrix (const vector <vector <float>>& filter)
{
    int filter_size = filter.size ();

    assert (filter_size == filter[0].size ());

    vector <vector <Pixel>> temp_pixels (width + 2, vector <Pixel> (height + 2));

    for (int x = 1; x <= width; ++x)
    {
        temp_pixels[x][height + 1] = pixels[x - 1][height - 1];
        temp_pixels[x][0]          = pixels[x - 1][0];
    }

    for (int y = 1; y <= height; ++y)
    {
        temp_pixels[width + 1][y] = pixels[width - 1][y - 1];
        temp_pixels[0][y]         = pixels[0][y - 1];
    }

    temp_pixels[0][0]          = pixels[0][0];
    temp_pixels[0][height + 1] = pixels[0][height - 1];
    temp_pixels[width + 1][0]          = pixels[width - 1][0];
    temp_pixels[width + 1][height + 1] = pixels[width - 1][height - 1];

    for (int x = 1; x <= width; ++x)
    {
        for (int y = 1; y <= height; ++y)
        {
            temp_pixels[x][y] = pixels[x - 1][y - 1];
        }
    }

    int start_offset = - (filter_size - 1) / 2;
    int end_offset   =   (filter_size - 1) / 2;


    for (int x = 1; x <= width; ++x)
    {
        for (int y = 1; y <= height; ++y)
        {
            float new_colour_r = 0;
            float new_colour_g = 0;
            float new_colour_b = 0;

            for (int i = start_offset; i <= end_offset; ++i)
            {
                for (int j = start_offset; j <= end_offset; ++j)
                {
                    new_colour_r += static_cast <float> (temp_pixels[x + i][y + j].r) * filter[i - start_offset][j - start_offset];
                    new_colour_g += static_cast <float> (temp_pixels[x + i][y + j].g) * filter[i - start_offset][j - start_offset];
                    new_colour_b += static_cast <float> (temp_pixels[x + i][y + j].b) * filter[i - start_offset][j - start_offset];
                }
            }

            if (new_colour_r < 0)
            {
                new_colour_r = 0;
            }
            if (new_colour_g < 0)
            {
                new_colour_g = 0;
            }
            if (new_colour_b < 0)
            {
                new_colour_b = 0;
            }

            if (new_colour_r > 255)
            {
                new_colour_r = 255;
            }
            if (new_colour_g > 255)
            {
                new_colour_g = 255;
            }
            if (new_colour_b > 255)
            {
                new_colour_b = 255;
            }

            pixels[x - 1][y - 1].r = static_cast <uint8_t> (new_colour_r);
            pixels[x - 1][y - 1].g = static_cast <uint8_t> (new_colour_g);
            pixels[x - 1][y - 1].b = static_cast <uint8_t> (new_colour_b);
        }
    }
}

ifstream& operator >> (ifstream& input, Image& image)
{
    int all_padding_size = image.dib_header.image_size - image.dib_header.width * image.dib_header.height * sizeof (Pixel);
    int padding_size = all_padding_size / image.dib_header.height;

    char padding[padding_size];
    memset (padding, 0, padding_size);

    for (int32_t y = image.height - 1; y >= 0; --y)
    {
        for (int32_t x = 0; x < image.width; ++x)
        {
            input.read (reinterpret_cast <char*> (&image.pixels[x][y]), sizeof (Pixel));
        }
        input.read (padding, padding_size);
    }

    char left_padding[all_padding_size - padding_size * image.dib_header.height];
    input.read (left_padding, all_padding_size - padding_size * image.dib_header.height);

    return input;
}

ofstream& operator << (ofstream& output, const Image& image)
{
    int padding_size = (4 - image.width * sizeof (Pixel) % 4) % 4;

    char padding[padding_size];
    memset (padding, 0, padding_size);

    for (int32_t y = image.height - 1; y >= 0; --y)
    {
        for (int32_t x = 0; x < image.width; ++x)
        {
            output.write (reinterpret_cast <const char*> (&image.pixels[x][y]), sizeof (Pixel));
        }
        output.write (padding, padding_size);
    }

    return output;
}