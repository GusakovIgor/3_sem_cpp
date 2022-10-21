#ifndef CONTAINER_HPP
#define CONTAINER_HPP


#include <string>

using std::string;


class Container;

typedef void (Container::*ContainerCommandPtr) (Command* command);

class Container
{

public:

	Container (const string& init_name);
	~Container ();

	void Execute (Command* command);

	void SetImage (const string& new_image_name, Image* new_image);
	Image* GetImage ();

	string Name ();
	string ImageName ();


	void Negative 		(Command* negative_base);
	void ReplaceColor 	(Command* replace_color_base);
	void ImproveClarity (Command* improve_clarity_base);
	void GaussianFilter (Command* gaussian_filter_base);
	void GreyFilter 	(Command* grey_filter_base);
	void EdgeDetection 	(Command* edge_detection_base);
	void ReduceNoise 	(Command* reduce_noise_base);
	void Vignette 		(Command* vignette_base);
	void Crop 			(Command* crop_base);
	void Compress 		(Command* compress_base);

private:

	string name;
	string image_name;

	Image* image;

	const unordered_map <CommandType, ContainerCommandPtr> operations =
	{
		{CommandType::cmd_negative, 		&Container::Negative 		},
		{CommandType::cmd_replace_color, 	&Container::ReplaceColor 	},
		{CommandType::cmd_improve_clarity, 	&Container::ImproveClarity 	},
		{CommandType::cmd_gaussian_filter, 	&Container::GaussianFilter 	},
		{CommandType::cmd_grey_filter, 		&Container::GreyFilter 		},
		{CommandType::cmd_edge_detection, 	&Container::EdgeDetection 	},
		{CommandType::cmd_reduce_noise, 	&Container::ReduceNoise 	},
		{CommandType::cmd_vignette, 		&Container::Vignette 		},
		{CommandType::cmd_crop, 			&Container::Crop 			},
		{CommandType::cmd_compress, 		&Container::Compress 		}
	};
};






#endif // CONTAINER_HPP