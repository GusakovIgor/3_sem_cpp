#ifndef SAVER_HPP
#define SAVER_HPP

#include <fstream>

using std::string;
using std::ofstream;


class Saver
{

public:

    Saver ();
    Saver (const int argc, const char* argv[]);

    void SaveImage (const string& image, Image* image_storage);

private:

    // config values

};

#endif  // SAVER_HPP