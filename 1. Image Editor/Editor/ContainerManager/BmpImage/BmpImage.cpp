#include "BmpImage.hpp"


// BmpHeader::BmpHeader ()
// {
//     type = 0x4d42;   // set (common, 'B' and 'M)
//     size = 0;        // empty (must be set)
//     reserved_1 = 0;  // set (common)
//     reserved_2 = 0;  // set (common)
//     offset = 0;      // empty (must be set)
// }


// BmpInfoHeader::BmpInfoHeader ()
// {
//     header_size = 40;
//     width  = 0;                             // empty (must be set)
//     height = 0;                             // empty (must be set)
//     planes = 1;                             // set (rule)
//     bits_per_pix = 0;                       // empty (must be set)
//     compression = 0;                        // set (not supported)
//     image_size = 0;                         // empty (must be set)
//     x_resolution = 2835;                    // set (common, 72 inches)
//     y_resolution = 2835;                    // set (common, 72 inches)
//     num_colours = 0;                        // set (not supported)
//     important_colours = 0;                  // set (not supported)
// }