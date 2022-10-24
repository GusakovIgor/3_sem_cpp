#include <BmpImage.hpp>
#include <Image.hpp>
#include "Loader.hpp"


Loader::Loader ()
{

}

Loader::Loader (const int argc, const char* argv[])
{

}


Image* Loader::LoadImage (const string& image)
{
    ifstream image_stream (image, std::ios::binary);

    if (!image_stream.is_open ())
    {
        // TODO: Proper error Handling
        std::cout << "Cannot open \"" << image << "\": No such file" << std::endl;
        return nullptr;
    }


    BmpHeader header;
    if (LoadBmpHeader (image, image_stream, &header) != 0)
    {
        return nullptr;
    }

    BmpInfoHeader dib_header;
    if (LoadBmpInfoHeader (image, image_stream, &dib_header) != 0)
    {
        return nullptr;
    }

    BmpColorHeader colour_header;

    if (dib_header.header_size - sizeof (BmpInfoHeader) == sizeof (BmpColorHeader))
    {
        if (LoadBmpColorHeader (image, image_stream, &colour_header) != 0)
        {
            return nullptr;
        }
    }

    Image* image_storage = new Image (header, dib_header, colour_header);

    image_stream.seekg (header.offset);
    image_stream >> *image_storage;

    image_stream.close ();

    return image_storage;
}

int32_t Loader::LoadBmpHeader (const string& image, ifstream& image_stream, BmpHeader* header)
{
    image_stream.read (reinterpret_cast <char*> (header), sizeof (BmpHeader));

    // std::cout << "header->type is " << std::hex << header->type << std::dec << std::endl;
    // std::cout << "header->size is " << header->size << std::endl;
    // std::cout << "header->reserved_1 is " << header->reserved_1 << std::endl;
    // std::cout << "header->reserved_2 is " << header->reserved_2 << std::endl;
    // std::cout << "header->offset is " << header->offset << std::endl;

    // 0x424d = 'B' 'M' (but real bytes reversed)
    if (header->type != 0x4d42)
    {
        // TODO: Proper error Handling
        std::cout << "Cannot open \"" << image << "\": Not a bmp file" << std::endl;
        return -1;
    }

    return 0;
}

int32_t Loader::LoadBmpInfoHeader (const string& image, ifstream& image_stream, BmpInfoHeader* dib_header)
{
    image_stream.read (reinterpret_cast <char*> (dib_header), sizeof (BmpInfoHeader));

    // std::cout << "dib_header->header_size is " << dib_header->header_size << std::endl;
    // std::cout << "dib_header->width is " << dib_header->width << std::endl;
    // std::cout << "dib_header->height is " << dib_header->height << std::endl;
    // std::cout << "dib_header->planes is " << dib_header->planes << std::endl;
    // std::cout << "dib_header->bits_per_pix is " << dib_header->bits_per_pix << std::endl;
    // std::cout << "dib_header->compression is " << dib_header->compression << std::endl;
    // std::cout << "dib_header->image_size is " << dib_header->image_size << std::endl;
    // std::cout << "dib_header->x_resolution is " << dib_header->x_resolution << std::endl;
    // std::cout << "dib_header->y_resolution is " << dib_header->y_resolution << std::endl;
    // std::cout << "dib_header->num_colours is " << dib_header->num_colours << std::endl;
    // std::cout << "dib_header->important_colours is " << dib_header->important_colours << std::endl;

    if (dib_header->bits_per_pix != sizeof (Pixel) * 8)
    {
        // TODO: Proper error Handling
        std::cout << "Cannot open \"" << image << "\": Wrong pixel format" << std::endl;
        return -1;
    }

    if (dib_header->compression != 0)
    {
        // TODO: Proper error Handling
        std::cout << "Cannot open \"" << image << "\": Compressed images not supported" << std::endl;
        return -1;
    }

    if (dib_header->num_colours != 0)
    {
        // TODO: Proper error Handling
        std::cout << "Cannot open \"" << image << "\": Images with colour table not supported" << std::endl;
        return -1;
    }

    return 0;
}

int32_t Loader::LoadBmpColorHeader (const string& image, ifstream& image_stream, BmpColorHeader* colour_header)
{
    image_stream.read (reinterpret_cast <char*> (colour_header), sizeof (BmpColorHeader));

    // std::cout << "colour_header->red_mask is   " << std::hex << colour_header->red_mask << std::dec << std::endl;
    // std::cout << "colour_header->green_mask is " << std::hex << colour_header->green_mask << std::dec << std::endl;
    // std::cout << "colour_header->blue_mask is  " << std::hex << colour_header->blue_mask << std::dec << std::endl;
    // std::cout << "colour_header->alpha_mask is " << std::hex << colour_header->alpha_mask << std::dec << std::endl;
    // std::cout << "colour_header->color_space_type is " << std::hex << colour_header->color_space_type << std::dec << std::endl;
    // std::cout << "colour_header->unused is " << std::hex << colour_header->unused << std::dec << std::endl;

    return 0;
}