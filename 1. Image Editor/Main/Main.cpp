#include <Image.hpp>
#include <Container.hpp>
#include <Commands.hpp>
#include <CommandFactory.hpp>
#include <InputManager.hpp>
#include <BmpImage.hpp>
#include <Loader.hpp>
#include <Saver.hpp>
#include <ContainerManager.hpp>
#include <Editor.hpp>


int main (const int argc, const char* argv[])
{
	Editor ImageEditor = Editor (argc, argv);

	ImageEditor ();

	return 0;
}