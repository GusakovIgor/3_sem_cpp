#include <Commands.hpp>
#include <CommandFactory.hpp>
#include <InputManager.hpp>
#include <Image.hpp>
#include <Container.hpp>
#include <BmpImage.hpp>
#include <Loader.hpp>
#include <Saver.hpp>
#include <ContainerManager.hpp>
#include <Editor.hpp>


int main (const int argc, const char* argv[])
{
	Editor image_editor = Editor (argc, argv);

	image_editor ();

	return 0;
}