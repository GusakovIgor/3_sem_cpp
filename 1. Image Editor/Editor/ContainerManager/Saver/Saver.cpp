#include <BmpImage.hpp>
#include <Image.hpp>
#include "Saver.hpp"


Saver::Saver ()
{

}

Saver::Saver (const int argc, const char* argv[])
{

}


void Saver::SaveImage (const string& image, Image* image_storage)
{
    ofstream image_stream (image, std::ios::binary & std::ios::trunc);

    if (!image_stream.is_open ())
    {
        // TODO: Proper error Handling
        std::cout << "Cannot open \"" << image << "\": No such file" << std::endl;
        return ;
    }


    int padding = (4 - image_storage->width * sizeof (Pixel) % 4) % 4;

    image_storage->header.size = sizeof (BmpHeader) + image_storage->dib_header.header_size + image_storage->Size ();
    image_storage->header.size += image_storage->height * padding;

    image_storage->dib_header.width  = image_storage->width;
    image_storage->dib_header.height = image_storage->height;
    image_storage->dib_header.image_size = image_storage->Size ();
    image_storage->dib_header.image_size += image_storage->height * padding;

    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << "header->type is " << std::hex << image_storage->header.type << std::dec << std::endl;
    // std::cout << "header->size is " << image_storage->header.size << std::endl;
    // std::cout << "header->reserved_1 is " << image_storage->header.reserved_1 << std::endl;
    // std::cout << "header->reserved_2 is " << image_storage->header.reserved_2 << std::endl;
    // std::cout << "header->offset is " << image_storage->header.offset << std::endl;

    // std::cout << "dib_header->header_size is " << image_storage->dib_header.header_size << std::endl;
    // std::cout << "dib_header->width is " << image_storage->dib_header.width << std::endl;
    // std::cout << "dib_header->height is " << image_storage->dib_header.height << std::endl;
    // std::cout << "dib_header->planes is " << image_storage->dib_header.planes << std::endl;
    // std::cout << "dib_header->bits_per_pix is " << image_storage->dib_header.bits_per_pix << std::endl;
    // std::cout << "dib_header->compression is " << image_storage->dib_header.compression << std::endl;
    // std::cout << "dib_header->image_size is " << image_storage->dib_header.image_size << std::endl;
    // std::cout << "dib_header->x_resolution is " << image_storage->dib_header.x_resolution << std::endl;
    // std::cout << "dib_header->y_resolution is " << image_storage->dib_header.y_resolution << std::endl;
    // std::cout << "dib_header->num_colours is " << image_storage->dib_header.num_colours << std::endl;
    // std::cout << "dib_header->important_colours is " << image_storage->dib_header.important_colours << std::endl;

    // std::cout << "colour_header->red_mask is   " << std::hex << image_storage->colour_header.red_mask << std::dec << std::endl;
    // std::cout << "colour_header->green_mask is " << std::hex << image_storage->colour_header.green_mask << std::dec << std::endl;
    // std::cout << "colour_header->blue_mask is  " << std::hex << image_storage->colour_header.blue_mask << std::dec << std::endl;
    // std::cout << "colour_header->alpha_mask is " << std::hex << image_storage->colour_header.alpha_mask << std::dec << std::endl;
    // std::cout << "colour_header->color_space_type is " << std::hex << image_storage->colour_header.color_space_type << std::dec << std::endl;
    // std::cout << "colour_header->unused is " << std::hex << image_storage->colour_header.unused << std::dec << std::endl;

    image_stream.write (reinterpret_cast <char*> (&image_storage->header), sizeof (BmpHeader));
    image_stream.write (reinterpret_cast <char*> (&image_storage->dib_header), sizeof (BmpInfoHeader));
    image_stream.write (reinterpret_cast <char*> (&image_storage->colour_header), image_storage->dib_header.header_size - sizeof (BmpInfoHeader));
    image_stream << *image_storage;

    image_stream.close ();
}