#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include <CommandFactory.hpp>
#include <InputManager.hpp>
#include <Container.hpp>
#include <Loader.hpp>
#include <Saver.hpp>
#include <ContainerManager.hpp>
#include <Editor.hpp>

#include <vector>
using std::vector;

int main (const int argc, const char* argv[])
{
	Editor image_editor = Editor (argc, argv);

	image_editor ();

/*	Saver* saver = new Saver ();

	Image* image = new Image (4, 4);

	vector <Pixel> image_pix =
	{
		Pixel (0xff, 0x00, 0x00),
		Pixel (0x00, 0xff, 0x00),
		Pixel (0x00, 0x00, 0xff),
		Pixel (0x00, 0x00, 0x00)
	};

	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			image->pixels[x][y] = image_pix[(x + y) % 4];
		}
	}

	// 0, 0 - red
	// 0, 1 - green
	// 0, 2 - blue
	// 0, 3 - black

	// 1, 0 - green
	// 1, 1 - blue
	// 1, 2 - black
	// 1, 3 - red

	// 2, 0 - blue
	// 2, 1 - black
	// 2, 2 - red
	// 2, 3 - green

	// 3, 0 - black
	// 3, 1 - red
	// 3, 2 - green
	// 3, 3 - blue
	std::cout << "Sizeof (BmpHeader) = " << sizeof (BmpHeader) << std::endl;
	std::cout << "Sizeof (BmpInfoHeader) = " << sizeof (BmpInfoHeader) << std::endl;


	Image* new_image = new Image (2, 2);
	new_image->header = image->header;
	new_image->dib_header = image->dib_header;
	new_image->colour_header = image->colour_header;

	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			std::cout << "Transfering (" << x << ", " << y << "): " << std::hex << (int) image->pixels[x][y].r << ", " << (int) image->pixels[x][y].g << ", " << (int) image->pixels[x][y].b << std::dec << std::endl;
			new_image->pixels[x][y] = image->pixels[x][y];
		}
	}

	// red, green
	// green, blue


	// green, 


	delete image;

	image = new_image;

	image->header.offset = sizeof (BmpHeader) + image->dib_header.header_size;

	saver->SaveImage ("Test/10.Crop/CroppedStalkingCat.bmp", image);

	delete saver;*/

	return 0;
}