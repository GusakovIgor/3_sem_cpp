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
    BmpHeader ();

    uint16_t type;          /* Magic identifier                         */
    uint32_t size;          /* File size in bytes                       */
    uint16_t reserved_1;    /* Depends on the application created image */
    uint16_t reserved_2;    /* Depends on the application created image */
    uint32_t offset;        /* Offset to image data, bytes              */

    int32_t RealSize () const;

};

ifstream& operator >> (ifstream& input,        BmpHeader& header);
ofstream& operator << (ofstream& output, const BmpHeader& header);


struct BmpInfoHeader
{
    BmpInfoHeader ();

    uint32_t header_size;       /* Header size in bytes    */
    int32_t  width;             /* Width of image          */
    int32_t  height;            /* Height of image         */
    uint16_t planes;            /* Number of colour planes */
    uint16_t bits_per_pix;      /* Bits per pixel          */
    uint32_t compression;       /* Compression type        */
    uint32_t image_size;        /* Image size in bytes     */
    int32_t  x_resolution;      /* Pixels per meter        */
    int32_t  y_resolution;      /* Pixels per meter        */
    uint32_t num_colours;       /* Number of colours       */
    uint32_t important_colours; /* Important colours       */

    int32_t RealSize () const;
};

ifstream& operator >> (ifstream& input,        BmpInfoHeader& dib_header);
ofstream& operator << (ofstream& output, const BmpInfoHeader& dib_header);

#endif  // BMP_IMAGE_HPP