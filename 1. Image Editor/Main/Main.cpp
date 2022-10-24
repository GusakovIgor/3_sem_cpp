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

    return 0;
}