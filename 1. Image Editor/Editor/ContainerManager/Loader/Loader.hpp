#ifndef LOADER_HPP
#define LOADER_HPP

#include <fstream>

using std::string;
using std::ifstream;


class Loader
{

public:

    Loader ();
    Loader (const int argc, const char* argv[]);

    Image* LoadImage (const string& image);

private:

    int32_t LoadBmpHeader     (const string& image, ifstream& image_stream, BmpHeader*     header);
    int32_t LoadBmpInfoHeader (const string& image, ifstream& image_stream, BmpInfoHeader* dib_header);

    // config values

};

#endif  // LOADER_HPP