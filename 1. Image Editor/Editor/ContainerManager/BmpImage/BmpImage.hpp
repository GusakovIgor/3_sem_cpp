#ifndef BMP_IMAGE_HPP
#define BMP_IMAGE_HPP

#include <iostream>
#include <fstream>
#include <cstdint>

using std::istream;
using std::ifstream;
using std::ostream;
using std::ofstream;


struct BmpHeader
{
    uint16_t type {0x4d42};     /* Magic identifier                         */
    uint32_t size {0};          /* File size in bytes                       */
    uint16_t reserved_1 {0};    /* Depends on the application created image */
    uint16_t reserved_2 {0};    /* Depends on the application created image */
    uint32_t offset {0};        /* Offset to image data, bytes              */
} __attribute ((__packed__));


struct BmpInfoHeader
{
    uint32_t header_size {40};      /* Header size in bytes    */
    int32_t  width  {0};            /* Width of image          */
    int32_t  height {0};            /* Height of image         */
    uint16_t planes {1};            /* Number of colour planes */
    uint16_t bits_per_pix {24};     /* Bits per pixel          */
    uint32_t compression {0};       /* Compression type        */
    uint32_t image_size {0};        /* Image size in bytes     */
    int32_t  x_resolution {2835};   /* Pixels per meter        */
    int32_t  y_resolution {2835};   /* Pixels per meter        */
    uint32_t num_colours {0};       /* Number of colours       */
    uint32_t important_colours {0}; /* Important colours       */
} __attribute ((__packed__));


struct BmpColorHeader
{
    uint32_t red_mask   {0x00ff0000};       // Bit mask for the red channel
    uint32_t green_mask {0x0000ff00};       // Bit mask for the green channel
    uint32_t blue_mask  {0x000000ff};       // Bit mask for the blue channel
    uint32_t alpha_mask {0xff000000};       // Bit mask for the alpha channel
    uint32_t color_space_type {0x73524742}; // Default "sRGB" (0x73524742)
    uint32_t unused[16] {0};                // Unused data for sRGB color space
} __attribute ((__packed__));


#endif  // BMP_IMAGE_HPP