#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

using std::istream;
using std::ifstream;
using std::ostream;
using std::ofstream;
using std::vector;


struct Pixel
{
    Pixel ();
    Pixel (const uint8_t init_r,
           const uint8_t init_g,
           const uint8_t init_b);

    uint8_t b;
    uint8_t g;
    uint8_t r;
} __attribute ((__packed__));

istream& operator >> (istream& input, Pixel& pixel);
bool operator == (const Pixel& left, const Pixel& right);


struct Image
{
    Image ();
    Image (const int32_t init_width,
           const int32_t init_height);

    Image (const BmpHeader& init_header,
           const BmpInfoHeader& init_dib_header,
           const BmpColorHeader& init_colour_header);

    int32_t Size () const;

    auto begin ();
    auto end ();

    void ApplyFilterMatrix (const vector <vector <float>>& filter);

    int32_t width;
    int32_t height;

    vector <vector <Pixel>> pixels;

    BmpHeader      header;
    BmpInfoHeader  dib_header;
    BmpColorHeader colour_header;

};

ifstream& operator >> (ifstream& input,        Image& image);
ofstream& operator << (ofstream& output, const Image& image);


#endif  // IMAGE_HPP