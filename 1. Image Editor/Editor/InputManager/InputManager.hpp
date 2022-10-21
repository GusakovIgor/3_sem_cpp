#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP


#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>



class InputManager
{

public:

	InputManager ();
	InputManager (const int argc, const char* argv[]);
	~InputManager ();

	Command* ParseCommand ();

private:

	std::streambuf* initial_input;
	std::stringstream actual_input;

	CommandFactory* command_factory;
};



// both in cmdline and interacive

const unordered_map <string, string> options_to_commands =
{
	{"-negative", 			"negative"			},
	{"-replace_color",		"replace_color"		},
	{"-improve_clarity", 	"improve_clarity"	},
	{"-gaussian_filter", 	"gaussian_filter"	},
	{"-grey_filter", 		"grey_filter"		},
	{"-edge_detection", 	"edge_detection"	},
	{"-reduce_noise", 		"reduce_noise"		},
	{"-vignette", 			"vignette"			},
	{"-crop", 				"crop"				},
	{"-compress", 			"compress"			},

	{"-load", "load"},
	{"-save", "save"},

	{"--help", "help"},
};


#endif // INPUT_MANAGER_HPP