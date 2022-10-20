#include "BmpImage.hpp"


BmpHeader::BmpHeader ()
{
    type = 0x4d42;   // set (common, 'B' and 'M)
    size = 0;        // empty (must be set)
    reserved_1 = 0;  // set (common)
    reserved_2 = 0;  // set (common)
    offset = 0;      // empty (must be set)
}

int32_t BmpHeader::RealSize () const
{
     return sizeof (type) +
            sizeof (size) +
            sizeof (reserved_1) +
            sizeof (reserved_2) +
            sizeof (offset);
}

ifstream& operator >> (ifstream& input, BmpHeader& header)
{
    input.read ((char*) (&header.type),       sizeof (header.type));
    input.read ((char*) (&header.size),       sizeof (header.size));
    input.read ((char*) (&header.reserved_1), sizeof (header.reserved_1));
    input.read ((char*) (&header.reserved_2), sizeof (header.reserved_2));
    input.read ((char*) (&header.offset),     sizeof (header.offset));

    return input;
}

ofstream& operator << (ofstream& output, const BmpHeader& header)
{
    output.write ((char*) (&header.type),       sizeof (header.type));
    output.write ((char*) (&header.size),       sizeof (header.size));
    output.write ((char*) (&header.reserved_1), sizeof (header.reserved_1));
    output.write ((char*) (&header.reserved_2), sizeof (header.reserved_2));
    output.write ((char*) (&header.offset),     sizeof (header.offset));

    return output;
}


BmpInfoHeader::BmpInfoHeader ()
{
    header_size = RealSize ();
    width  = 0;                             // empty (must be set)
    height = 0;                             // empty (must be set)
    planes = 1;                             // set (rule)
    bits_per_pix = 0;                       // empty (must be set)
    compression = 0;                        // set (not supported)
    image_size = 0;                         // empty (must be set)
    x_resolution = 2835;                    // set (common, 72 inches)
    y_resolution = 2835;                    // set (common, 72 inches)
    num_colours = 0;                        // set (not supported)
    important_colours = 0;                  // set (not supported)
}

int32_t BmpInfoHeader::RealSize () const
{
     return sizeof (header_size) +
            sizeof (width) +
            sizeof (height) +
            sizeof (planes) +
            sizeof (bits_per_pix) +
            sizeof (compression) +
            sizeof (image_size) +
            sizeof (x_resolution) +
            sizeof (y_resolution) +
            sizeof (num_colours) +
            sizeof (important_colours);
}

ifstream& operator >> (ifstream& input, BmpInfoHeader& dib_header)
{
    input.read ((char*) (&dib_header.header_size),       sizeof (dib_header.header_size));
    input.read ((char*) (&dib_header.width),             sizeof (dib_header.width));
    input.read ((char*) (&dib_header.height),            sizeof (dib_header.height));
    input.read ((char*) (&dib_header.planes),            sizeof (dib_header.planes));
    input.read ((char*) (&dib_header.bits_per_pix),      sizeof (dib_header.bits_per_pix));
    input.read ((char*) (&dib_header.compression),       sizeof (dib_header.compression));
    input.read ((char*) (&dib_header.image_size),        sizeof (dib_header.image_size));
    input.read ((char*) (&dib_header.x_resolution),      sizeof (dib_header.x_resolution));
    input.read ((char*) (&dib_header.y_resolution),      sizeof (dib_header.y_resolution));
    input.read ((char*) (&dib_header.num_colours),       sizeof (dib_header.num_colours));
    input.read ((char*) (&dib_header.important_colours), sizeof (dib_header.important_colours));

    return input;
}

ofstream& operator << (ofstream& output, const BmpInfoHeader& dib_header)
{
    output.write ((char*) (&dib_header.header_size),       sizeof (dib_header.header_size));
    output.write ((char*) (&dib_header.width),             sizeof (dib_header.width));
    output.write ((char*) (&dib_header.height),            sizeof (dib_header.height));
    output.write ((char*) (&dib_header.planes),            sizeof (dib_header.planes));
    output.write ((char*) (&dib_header.bits_per_pix),      sizeof (dib_header.bits_per_pix));
    output.write ((char*) (&dib_header.compression),       sizeof (dib_header.compression));
    output.write ((char*) (&dib_header.image_size),        sizeof (dib_header.image_size));
    output.write ((char*) (&dib_header.x_resolution),      sizeof (dib_header.x_resolution));
    output.write ((char*) (&dib_header.y_resolution),      sizeof (dib_header.y_resolution));
    output.write ((char*) (&dib_header.num_colours),       sizeof (dib_header.num_colours));
    output.write ((char*) (&dib_header.important_colours), sizeof (dib_header.important_colours));

    return output;
}