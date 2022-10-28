#include <BmpImage.hpp>
#include <Image.hpp>
#include <Commands.hpp>
#include "Container.hpp"

#include <algorithm>
#include <functional>
#include <cassert>
#include <cstring>
#include <cmath>


Container::Container (const string& init_name)
{
    name = init_name;

    image = nullptr;
}

Container::~Container ()
{
    if (image)
    {
        delete image;
    }

    while (!applied_commands.empty ())
    {
        Command* command = applied_commands.front ();
        applied_commands.pop ();

        delete command;
    }
}


void Container::SetImage (const string& new_image_name, Image* new_image)
{
    image = new_image;

    image_name = new_image_name;
}

Image* Container::GetImage ()
{
    return image;
}


string Container::Name ()
{
    return name;
}

string Container::ImageName ()
{
    return image_name;
}



void Container::Execute (Command* command)
{
    if (operations.contains (command->Type ()))
    {
        command->SetPrevImage (image);

        if (std::invoke (operations.at (command->Type ()), *this, command))
        {
            applied_commands.push (command);
        }
    }
    else
    {
        std::cout << "Implementing of this command is currently in progress" << std::endl;
    }
}

void Container::Cancel ()
{
    if (!applied_commands.empty ())
    {
        Command* command = applied_commands.front ();
        applied_commands.pop ();

        if (command->GetPrevImage ())
        {
            delete image;

            image = command->GetPrevImage ();
        }
    }
}



bool Container::Negative (Command* negative_base)
{
    for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
            image->pixels[x][y].r = 255 - image->pixels[x][y].r;
            image->pixels[x][y].g = 255 - image->pixels[x][y].g;
            image->pixels[x][y].b = 255 - image->pixels[x][y].b;
        }
    }

    return true;
}


bool Container::ReplaceColor (Command* replace_color_base)
{
    ReplaceColorCmd* replace_colour = dynamic_cast <ReplaceColorCmd*> (replace_color_base);

    for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
            if (image->pixels[x][y] == replace_colour->TargetColour ())
            {
                image->pixels[x][y] = replace_colour->NewColour ();
            }
        }
    }

    return true;
}


bool Container::ImproveClarity (Command* improve_clarity_base)
{
    vector <vector <float>> filter_matrix (3, vector <float> (3, -1.0));

    filter_matrix[1][1] = 9.0;

    image->ApplyFilterMatrix (filter_matrix);

    return true;
}


bool Container::GaussianFilter (Command* gaussian_filter_base)
{
    vector <vector <float>> filter_matrix =
    {
        {0.054901, 0.111345, 0.054901},
        {0.111345, 0.225821, 0.111345},
        {0.054901, 0.111345, 0.054901}
    };

    image->ApplyFilterMatrix (filter_matrix);

    return true;
}


bool Container::GreyFilter (Command* grey_filter_base)
{
    for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
            uint32_t average_component = (image->pixels[x][y].r +
                                          image->pixels[x][y].g +
                                          image->pixels[x][y].b) / 3;

            image->pixels[x][y] = Pixel (average_component, average_component, average_component);
        }
    }

    return true;
}


bool Container::EdgeDetection (Command* edge_detection_base)
{
    EdgeDetectionCmd* edge_detection = dynamic_cast <EdgeDetectionCmd*> (edge_detection_base);

    uint32_t detection_border = 255 - 255 * edge_detection->DetectionPercent () / 100;


    GreyFilter (nullptr);

    Image* temp_image = new Image ();
    *temp_image = *image;

    vector <vector <float>> first_sobel_matrix =
    {
        {-1.0, -2.0, -1.0},
        { 0.0,  0.0,  0.0},
        { 1.0,  2.0,  1.0}
    };

    image->ApplyFilterMatrix (first_sobel_matrix);

    vector <vector <float>> second_sobel_matrix =
    {
        {-1.0, 0.0, 1.0},
        {-2.0, 0.0, 2.0},
        {-1.0, 0.0, 1.0}
    };

    temp_image->ApplyFilterMatrix (second_sobel_matrix);

    for (uint32_t y = 0; y < image->height; ++y)
    {
        for (uint32_t x = 0; x < image->width; ++x)
        {
            uint32_t new_r = std::min (255u, static_cast <uint32_t> (sqrt (image->pixels[x][y].r * image->pixels[x][y].r + temp_image->pixels[x][y].r * temp_image->pixels[x][y].r)));
            uint32_t new_g = std::min (255u, static_cast <uint32_t> (sqrt (image->pixels[x][y].g * image->pixels[x][y].g + temp_image->pixels[x][y].g * temp_image->pixels[x][y].g)));
            uint32_t new_b = std::min (255u, static_cast <uint32_t> (sqrt (image->pixels[x][y].b * image->pixels[x][y].b + temp_image->pixels[x][y].b * temp_image->pixels[x][y].b)));

            if (new_r <= detection_border ||
                new_g <= detection_border ||
                new_b <= detection_border)
            {
                new_r = new_g = new_b = 0;
            }

            image->pixels[x][y] = Pixel (new_r, new_g, new_b);
        }
    }

    delete temp_image;

    return true;
}


bool Container::ReduceNoise (Command* reduce_noise_base)
{
    ReduceNoiseCmd* reduce_noise = dynamic_cast <ReduceNoiseCmd*> (reduce_noise_base);

    int reduce_matrix_size = 10 * reduce_noise->ReducePercent () / 100;

    if (reduce_matrix_size == 0)
    {
        return true;
    }

    vector <vector <Pixel>> temp_pixels (image->width, vector <Pixel> (image->height));

    int start_offset = - (reduce_matrix_size - 1) / 2;
    int end_offset   =   (reduce_matrix_size - 1) / 2;

    for (int x = 0; x < image->width; ++x)
    {
        for (int y = 0; y < image->height; ++y)
        {
            vector <uint8_t> surroundings_r;
            vector <uint8_t> surroundings_g;
            vector <uint8_t> surroundings_b;

            for (int i = start_offset; i <= end_offset; ++i)
            {
                for (int j = start_offset; j <= end_offset; ++j)
                {
                    if (x + i < 0 ||
                        y + j < 0 ||
                        x + i >= image->width ||
                        y + j >= image->height)
                    {
                        continue;
                    }

                    surroundings_r.push_back (image->pixels[x + i][y + j].r);
                    surroundings_g.push_back (image->pixels[x + i][y + j].g);
                    surroundings_b.push_back (image->pixels[x + i][y + j].b);
                }
            }

            std::sort (surroundings_r.begin (), surroundings_r.end ());
            std::sort (surroundings_g.begin (), surroundings_g.end ());
            std::sort (surroundings_b.begin (), surroundings_b.end ());

            uint32_t median_index = (surroundings_r.size () - 1) / 2;

            temp_pixels[x][y] = Pixel (surroundings_r[median_index], surroundings_g[median_index], surroundings_b[median_index]);
        }
    }

    for (int x = 0; x < image->width; ++x)
    {
        for (int y = 0; y < image->height; ++y)
        {
            image->pixels[x][y] = temp_pixels[x][y];
        }
    }

    return true;
}

bool Container::Vignette (Command* vignette_base)
{
    VignetteCmd* vignette = dynamic_cast <VignetteCmd*> (vignette_base);

    Pixel vignette_colour = vignette->Colour ();

    double size_factor = sqrt (2.0) * 0.5;

    double vignette_width  = size_factor * image->width  * (vignette->Coverage () / 100.0);
    double vignette_height = size_factor * image->height * (vignette->Coverage () / 100.0);

    double blackout_size = 0.0;

    if (vignette->Blackout () == 100.0)
    {
        blackout_size = vignette_height;
    }
    else if (vignette->Blackout () == 0.0)
    {
        blackout_size = 0;
    }
    else
    {
        blackout_size = GetBlackoutSize (vignette->Blackout ());
    }

    double blend_size_x = vignette_width  - blackout_size;
    double blend_size_y = vignette_height - blackout_size;

    int num_steps = static_cast <int> (std::min (100.0, std::min (blend_size_x / 2.0, blend_size_y / 2.0)));

    double step_size_x = (num_steps == 0) ? 0 : blend_size_x / static_cast <double> (num_steps);
    double step_size_y = (num_steps == 0) ? 0 : blend_size_y / static_cast <double> (num_steps);

    vector <double> ring_start_x;
    vector <double> ring_start_y;
    vector <double> weight_original;
    vector <double> weight_vignette;

    GetVignetteRingsAndWeights (num_steps,
                                step_size_x,
                                step_size_y,
                                vignette_width,
                                vignette_height,
                                ring_start_x,
                                ring_start_y,
                                weight_original,
                                weight_vignette);

    for (int x = 0; x < image->width; ++x)
    {
        for (int y = 0; y < image->height; ++y)
        {
            double ellips_related_x = x - image->width / 2;
            double ellips_related_y = y - image->height / 2;
            double factor1 = abs (ellips_related_x) / ring_start_x[0];
            double factor2 = abs (ellips_related_y) / ring_start_y[0];
            double factor3 = abs (ellips_related_x) / ring_start_x[num_steps];
            double factor4 = abs (ellips_related_y) / ring_start_y[num_steps];

            // Ellipse equations.
            // These are distances from the inner and outer boundaries of the two ellipses.
            double inner_ellips_dist = factor1 * factor1 + factor2 * factor2;
            double outer_ellips_dist = factor3 * factor3 + factor4 * factor4;

            if (outer_ellips_dist >= 1.0)
            {
                // Point is outside the outer ellipse
                image->pixels[x][y] = vignette_colour;
            }
            else if (inner_ellips_dist >= 1.0)
            {
                // Point is outside the inner ellipse, i.e.
                // Point is in between the outermost and innermost ellipses
                int ring_num = 0;

                for (ring_num = 1; ring_num < num_steps; ++ring_num)
                {
                    double factor1 = abs (ellips_related_x) / ring_start_x[ring_num];
                    double factor2 = abs (ellips_related_y) / ring_start_y[ring_num];

                    double ring_num_ellips_dist = factor1 * factor1 + factor2 * factor2;

                    if (ring_num_ellips_dist < 1.0)
                    {
                        break;
                    }
                }

                int current_ring_num = ring_num - 1;

                image->pixels[x][y].r = (image->pixels[x][y].r * weight_original[current_ring_num] +
                                         vignette_colour.r     * weight_vignette[current_ring_num]);
                image->pixels[x][y].g = (image->pixels[x][y].g * weight_original[current_ring_num] +
                                         vignette_colour.g     * weight_vignette[current_ring_num]);
                image->pixels[x][y].b = (image->pixels[x][y].b * weight_original[current_ring_num] +
                                         vignette_colour.b     * weight_vignette[current_ring_num]);
            }
        }
    }

    return true;
}

double Container::GetBlackoutSize (double blackout_percent)
{
    static const double size_factor = sqrt (2.0) * 0.5;

    double blackout_size = 0.0;

    double border_point_x = 0.5 * image->width  * sqrt (1.0 - blackout_percent / 100.0);
    double border_point_y = 0.5 * image->height * sqrt (1.0 - blackout_percent / 100.0);

    double factor_x = border_point_x / (size_factor * image->width  - blackout_size);
    double factor_y = border_point_y / (size_factor * image->height - blackout_size);

    while (factor_x * factor_x + factor_y * factor_y <= 1)
    {
        ++blackout_size;

        factor_x = border_point_x / (size_factor * image->width  - blackout_size);
        factor_y = border_point_y / (size_factor * image->height - blackout_size);
    }

    return blackout_size;
}

void Container::GetVignetteRingsAndWeights (int num_steps,
                                            double step_size_x,
                                            double step_size_y,
                                            double vignette_width,
                                            double vignette_height,
                                            vector <double>& ring_start_x,
                                            vector <double>& ring_start_y,
                                            vector <double>& weight_original,
                                            vector <double>& weight_vignette)
{
    static const double size_factor = sqrt (2.0) * 0.5;

    ring_start_x.assign (num_steps + 1, 0);
    ring_start_y.assign (num_steps + 1, 0);
    vector <double> ring_middle_x (num_steps, 0);
    vector <double> ring_middle_y (num_steps, 0);

    ring_start_x[0] = size_factor * image->width  - vignette_width;
    ring_start_y[0] = size_factor * image->height - vignette_height;

    for (int i = 1; i <= num_steps; ++i)
    {
        ring_start_x[i] = (ring_start_x[0] + i * step_size_x);
        ring_start_y[i] = (ring_start_y[0] + i * step_size_y);
        ring_middle_x[i - 1] = (ring_start_x[0] + (i - 0.5) * step_size_x);
        ring_middle_y[i - 1] = (ring_start_y[0] + (i - 0.5) * step_size_y);
    }


    weight_original.assign (num_steps, 0.0);
    weight_vignette.assign (num_steps, 0.0);

    double blend_size = num_steps * step_size_x;
    double cos_arg_factor = (step_size_x == 0) ? 0 : M_PI / blend_size;

    for (int i = 0; i < num_steps; ++i)
    {
        double cos_arg = 0.8 * (ring_middle_x[i] - ring_start_x[0]) * cos_arg_factor;
        double cos_val = cos (cos_arg);

        weight_original[i] = 0.5 * (1 + cos_val);
        weight_vignette[i] = 0.5 * (1 - cos_val);
    }
}


bool Container::Crop (Command* crop_base)
{
    CropCmd* crop = dynamic_cast <CropCmd*> (crop_base);

    if (crop->StartX ()    < 0 ||
        crop->StartY ()    < 0 ||
        crop->NewWidth ()  < 0 ||
        crop->NewHeight () < 0)
    {
        std::cout << "Seriously? Negative crop?.. Think of something better to test with." << std::endl << std::endl;

        return false;
    }

    if (crop->StartX () + crop->NewWidth () > image->width)
    {
        std::cout << "Wrong crop: current image width is " << image->width << std::endl << std::endl;

        return false;
    }

    if (crop->StartY () + crop->NewHeight () > image->height)
    {
        std::cout << "Wrong crop: current image height is " << image->height << std::endl << std::endl;

        return false;
    }

    Image* new_image = new Image (crop->NewWidth (), crop->NewHeight ());
    new_image->header = image->header;
    new_image->dib_header = image->dib_header;
    new_image->colour_header = image->colour_header;

    for (int x = 0; x < crop->NewWidth (); ++x)
    {
        for (int y = 0; y < crop->NewHeight (); ++y)
        {
            new_image->pixels[x][y] = image->pixels[crop->StartX () + x][crop->StartY () + y];
        }
    }

    image = new_image;

    return true;
}


bool Container::Downscale (Command* downscale_base)
{
    DownscaleCmd* downscale = dynamic_cast <DownscaleCmd*> (downscale_base);

    vector <vector <Pixel>> new_image (downscale->NewWidth (), vector <Pixel> (downscale->NewHeight (), Pixel (0, 0, 0)));

    double target_step_x = downscale->NewWidth  () / static_cast <double> (image->width);
    double target_step_y = downscale->NewHeight () / static_cast <double> (image->height);

    double target_ready_x = 1.0;
    double target_ready_y = 1.0;

    int target_total_ready_x = 0;
    int target_total_ready_y = 0;

    for (int x = 0; x < image->width; ++x)
    {
        if (target_total_ready_x == downscale->NewWidth ())
        {
            break;
        }

        if (target_ready_x > 1.0)
        {
            for (int y = 0; y < image->height; ++y)
            {
                if (target_total_ready_y == downscale->NewHeight ())
                {
                    break;
                }

                if (target_ready_y > 1.0)
                {
                    int average_r = 0;
                    int average_g = 0;
                    int average_b = 0;

                    int block_width  = static_cast <int> (ceil (target_ready_x / target_step_x));
                    int block_height = static_cast <int> (ceil (target_ready_y / target_step_y));

                    int count = 0;

                    for (int i = 0; i < block_width; ++i)
                    {
                        for (int j = 0; j < block_height; ++j)
                        {
                            if (x - i >= 0 &&
                                y - j >= 0)
                            {
                                average_r += image->pixels[x - i][y - j].r;
                                average_g += image->pixels[x - i][y - j].g;
                                average_b += image->pixels[x - i][y - j].b;

                                ++count;
                            }
                        }
                    }

                    new_image[target_total_ready_x][target_total_ready_y].r = average_r / count;
                    new_image[target_total_ready_x][target_total_ready_y].g = average_g / count;
                    new_image[target_total_ready_x][target_total_ready_y].b = average_b / count;

                    target_ready_y -= 1.0;
                    ++target_total_ready_y;
                }

                target_ready_y += target_step_y;
            }

            target_ready_y = 1.0;
            target_total_ready_y = 0;

            target_ready_x -= 1.0;
            ++target_total_ready_x;
        }

        target_ready_x += target_step_x;
    }

    image->pixels = new_image;
    image->width  = downscale->NewWidth ();
    image->height = downscale->NewHeight ();

    return true;
}