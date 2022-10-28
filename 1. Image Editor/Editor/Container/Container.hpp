#ifndef CONTAINER_HPP
#define CONTAINER_HPP


#include <string>
#include <queue>

using std::string;
using std::queue;


class Container;

typedef bool (Container::*ContainerCmdPtr) (Command* command);

class Container
{

public:

    Container (const string& init_name);
    ~Container ();

    void SetImage (const string& new_image_name, Image* new_image);
    Image* GetImage ();

    string Name ();
    string ImageName ();

    void Execute (Command* command);
    void Cancel  ();

private:

    string name;
    string image_name;

    Image* image;
    queue <Command*> applied_commands;

    bool Negative       (Command* negative_base);
    bool ReplaceColor   (Command* replace_color_base);
    bool ImproveClarity (Command* improve_clarity_base);
    bool GaussianFilter (Command* gaussian_filter_base);
    bool GreyFilter     (Command* grey_filter_base);
    bool EdgeDetection  (Command* edge_detection_base);
    bool ReduceNoise    (Command* reduce_noise_base);
    bool Vignette       (Command* vignette_base);
    bool Crop           (Command* crop_base);
    bool Downscale      (Command* downscale_base);

    double GetBlackoutSize (double blackout_percent);

    void GetVignetteRingsAndWeights (int num_steps,
                                     double step_size_x,
                                     double step_size_y,
                                     double vignette_width,
                                     double vignette_height,
                                     vector <double>& ring_start_x,
                                     vector <double>& ring_start_y,
                                     vector <double>& weight_original,
                                     vector <double>& weight_vignette);

    const unordered_map <CommandType, ContainerCmdPtr> operations =
    {
        {CommandType::cmd_negative,         &Container::Negative        },
        {CommandType::cmd_replace_color,    &Container::ReplaceColor    },
        {CommandType::cmd_improve_clarity,  &Container::ImproveClarity  },
        {CommandType::cmd_gaussian_filter,  &Container::GaussianFilter  },
        {CommandType::cmd_grey_filter,      &Container::GreyFilter      },
        {CommandType::cmd_edge_detection,   &Container::EdgeDetection   },
        {CommandType::cmd_reduce_noise,     &Container::ReduceNoise     },
        {CommandType::cmd_vignette,         &Container::Vignette        },
        {CommandType::cmd_downscale,        &Container::Downscale       },
        {CommandType::cmd_crop,             &Container::Crop            }
    };
};






#endif // CONTAINER_HPP