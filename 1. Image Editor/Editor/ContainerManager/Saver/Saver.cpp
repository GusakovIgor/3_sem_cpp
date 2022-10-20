#include <Image.hpp>
#include <BmpImage.hpp>
#include "Saver.hpp"


Saver::Saver ()
{

}

Saver::Saver (const int argc, const char* argv[])
{

}


void Saver::SaveImage (const string& image, const Image* image_storage)
{
	ofstream image_stream (image, std::ios::binary & std::ios::trunc);

	if (!image_stream.is_open ())
	{
		// TODO: Proper error Handling
		std::cout << "Cannot open \"" << image << "\": No such file" << std::endl;
		return ;
	}

	BmpHeader header;
	BmpInfoHeader dib_header;

	header.size = header.RealSize () + dib_header.RealSize () + image_storage->Size ();
	header.offset = header.RealSize () + dib_header.RealSize ();

	dib_header.width  = image_storage->width;
	dib_header.height = image_storage->height;
	dib_header.bits_per_pix = sizeof (Pixel) * 8;
	dib_header.image_size = image_storage->Size ();


	// std::cout << std::endl;
	// std::cout << std::endl;
	// std::cout << "header->type is " << std::hex << header.type << std::endl;
	// std::cout << "header->size is " << header.size << std::endl;
	// std::cout << "header->reserved_1 is " << header.reserved_1 << std::endl;
	// std::cout << "header->reserved_2 is " << header.reserved_2 << std::endl;
	// std::cout << "header->offset is " << header.offset << std::endl;

	// std::cout << "dib_header->header_size is " << dib_header.header_size << std::endl;
	// std::cout << "dib_header->width is " << dib_header.width << std::endl;
	// std::cout << "dib_header->height is " << dib_header.height << std::endl;
	// std::cout << "dib_header->planes is " << dib_header.planes << std::endl;
	// std::cout << "dib_header->bits_per_pix is " << dib_header.bits_per_pix << std::endl;
	// std::cout << "dib_header->compression is " << dib_header.compression << std::endl;
	// std::cout << "dib_header->image_size is " << dib_header.image_size << std::endl;
	// std::cout << "dib_header->x_resolution is " << dib_header.x_resolution << std::endl;
	// std::cout << "dib_header->y_resolution is " << dib_header.y_resolution << std::endl;
	// std::cout << "dib_header->num_colours is " << dib_header.num_colours << std::endl;
	// std::cout << "dib_header->important_colours is " << dib_header.important_colours << std::endl;


	image_stream << header;
	image_stream << dib_header;
	image_stream << *image_storage;

	image_stream.close ();
}