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

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

ifstream& operator >> (ifstream& input,        Pixel& pixel);
ofstream& operator << (ofstream& output, const Pixel& pixel);


struct Image
{
    Image ();
    Image (const int32_t init_width,
           const int32_t init_height);

    auto begin ();
    auto end ();

    int32_t width;
    int32_t height;

    vector <vector <Pixel>> pixels;

    int32_t Size () const;
};

ifstream& operator >> (ifstream& input,        Image& image);
ofstream& operator << (ofstream& output, const Image& image);


#endif  // IMAGE_HPP