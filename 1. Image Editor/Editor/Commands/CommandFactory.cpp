#include <Image.hpp>
#include <Container.hpp>
#include "Commands.hpp"
#include "CommandFactory.hpp"

Command* CommandFactory::New (const string& command_name)
{
	if (constructor.contains (command_name))
	{
		cmd_ctr_t command_type_constructor = constructor.at(command_name);

		return std::invoke (command_type_constructor);
	}

	return nullptr;
}

Command* CommandFactory::NewNegativeCmd ()
{
	return new NegativeCmd ();
}

Command* CommandFactory::NewReplaceColorCmd ()
{
	return new ReplaceColorCmd ();
}

Command* CommandFactory::NewImproveClarityCmd ()
{
	return new ImproveClarityCmd ();
}

Command* CommandFactory::NewGaussianFilterCmd ()
{
	return new GaussianFilterCmd ();
}

Command* CommandFactory::NewGreyFilterCmd ()
{
	return new GreyFilterCmd ();
}

Command* CommandFactory::NewEdgeDetectionCmd ()
{
	return new EdgeDetectionCmd ();
}

Command* CommandFactory::NewReduceNoiseCmd ()
{
	return new ReduceNoiseCmd ();
}

Command* CommandFactory::NewVignetteCmd ()
{
	return new VignetteCmd ();
}

Command* CommandFactory::NewCropCmd ()
{
	return new CropCmd ();
}

Command* CommandFactory::NewCompressCmd ()
{
	return new CompressCmd ();
}

Command* CommandFactory::NewLoadCmd ()
{
	return new LoadCmd ();
}

Command* CommandFactory::NewSaveCmd ()
{
	return new SaveCmd ();
}

Command* CommandFactory::NewListContainersCmd ()
{
	return new ListContainersCmd ();
}

Command* CommandFactory::NewAddContainerCmd ()
{
	return new AddContainerCmd ();
}

Command* CommandFactory::NewDelContainerCmd ()
{
	return new DelContainerCmd ();
}

Command* CommandFactory::NewSwitchContainerCmd ()
{
	return new SwitchContainerCmd ();
}

Command* CommandFactory::NewExitCmd ()
{
	return new ExitCmd ();
}

Command* CommandFactory::NewHelpCmd ()
{
	return new HelpCmd ();
}
