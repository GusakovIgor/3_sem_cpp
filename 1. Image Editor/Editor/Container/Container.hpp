#ifndef CONTAINER_HPP
#define CONTAINER_HPP


#include <string>
#include <queue>

using std::string;
using std::queue;


class Container;

typedef bool (Container::*ContainerCmdPtr) (Command* command);
typedef void (Container::*ContainerRevCmdPtr) (Command* command);

class Container
{

public:

	Container (const string& init_name);
	~Container ();

	void Execute (Command* command);
	void Cancel  ();

	void SetImage (const string& new_image_name, Image* new_image);
	Image* GetImage ();

	string Name ();
	string ImageName ();

private:

	string name;
	string image_name;

	Image* image;
	queue <Command*> applied_commands;

	bool Negative 		(Command* negative_base);
	bool ReplaceColor 	(Command* replace_color_base);
	bool ImproveClarity (Command* improve_clarity_base);
	bool GaussianFilter (Command* gaussian_filter_base);
	bool GreyFilter 	(Command* grey_filter_base);
	bool EdgeDetection 	(Command* edge_detection_base);
	bool ReduceNoise 	(Command* reduce_noise_base);
	bool Vignette 		(Command* vignette_base);
	bool Crop 			(Command* crop_base);
	bool Compress 		(Command* compress_base);

	const unordered_map <CommandType, ContainerCmdPtr> operations =
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

	void RevNegative 		(Command* negative_base);
	void RevReplaceColor 	(Command* replace_color_base);
	void RevImproveClarity 	(Command* improve_clarity_base);
	void RevGaussianFilter 	(Command* gaussian_filter_base);
	void RevGreyFilter 		(Command* grey_filter_base);
	void RevEdgeDetection 	(Command* edge_detection_base);
	void RevReduceNoise 	(Command* reduce_noise_base);
	void RevVignette 		(Command* vignette_base);
	void RevCrop 			(Command* crop_base);
	void RevCompress 		(Command* compress_base);

	const unordered_map <CommandType, ContainerRevCmdPtr> rev_operations =
	{
		{CommandType::cmd_negative, 		&Container::RevNegative 		},
		{CommandType::cmd_replace_color, 	&Container::RevReplaceColor 	},
		{CommandType::cmd_improve_clarity, 	&Container::RevImproveClarity 	},
		{CommandType::cmd_gaussian_filter, 	&Container::RevGaussianFilter 	},
		{CommandType::cmd_grey_filter, 		&Container::RevGreyFilter 		},
		{CommandType::cmd_edge_detection, 	&Container::RevEdgeDetection 	},
		{CommandType::cmd_reduce_noise, 	&Container::RevReduceNoise 		},
		{CommandType::cmd_vignette, 		&Container::RevVignette 		},
		{CommandType::cmd_crop, 			&Container::RevCrop 			},
		{CommandType::cmd_compress, 		&Container::RevCompress 		}
	};
};






#endif // CONTAINER_HPP