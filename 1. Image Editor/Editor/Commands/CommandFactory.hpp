#include <functional>

typedef Command* (*cmd_ctr_t) ();

class CommandFactory
{

public:

	Command* New (const string& command_name);

private:

	static Command* NewNegativeCmd ();
	static Command* NewReplaceColorCmd ();
	static Command* NewImproveClarityCmd ();
	static Command* NewGaussianFilterCmd ();
	static Command* NewGreyFilterCmd ();
	static Command* NewEdgeDetectionCmd ();
	static Command* NewReduceNoiseCmd ();
	static Command* NewVignetteCmd ();
	static Command* NewCropCmd ();
	static Command* NewCompressCmd ();
	static Command* NewLoadCmd ();
	static Command* NewSaveCmd ();
	static Command* NewListContainersCmd ();
	static Command* NewAddContainerCmd ();
	static Command* NewDelContainerCmd ();
	static Command* NewSwitchContainerCmd ();
	static Command* NewExitCmd ();
	static Command* NewHelpCmd ();

	inline static const unordered_map <string, cmd_ctr_t> constructor
	{
		{"negative", 		CommandFactory::NewNegativeCmd},
		{"replace_color", 	CommandFactory::NewReplaceColorCmd}, // from to
		{"improve_clarity",	CommandFactory::NewImproveClarityCmd},
		{"gaussian_filter", CommandFactory::NewGaussianFilterCmd},
		{"grey_filter", 	CommandFactory::NewGreyFilterCmd},
		{"edge_detection",  CommandFactory::NewEdgeDetectionCmd},
		{"reduce_noise", 	CommandFactory::NewReduceNoiseCmd},
		{"vignette", 		CommandFactory::NewVignetteCmd},
		{"crop", 			CommandFactory::NewCropCmd}, // (x,y,width, height)
		{"compress", 		CommandFactory::NewCompressCmd}, // a_new, b_new

		{"load", CommandFactory::NewLoadCmd},
		{"save", CommandFactory::NewSaveCmd},

		{"list",   CommandFactory::NewListContainersCmd},
		{"add",    CommandFactory::NewAddContainerCmd},
		{"del",    CommandFactory::NewDelContainerCmd},
		{"switch", CommandFactory::NewSwitchContainerCmd},

		{"exit", CommandFactory::NewExitCmd},
		{"help", CommandFactory::NewHelpCmd}
	};
};