#include "libCFG/setup/parametersource/CommandLineParameterSource.h"
#include "libCFG/setup/parametersource/anyoption.h"
#include "libCFG/error/ParameterNotFoundException.h"

namespace libCFG
{
    CommandLineParameterSource::CommandLineParameterSource()
    {
        options = new AnyOption();
    }

    CommandLineParameterSource::CommandLineParameterSource(int argc, char** argv)
        : argc(argc), argv(argv)
    {
        options = new AnyOption();
    }

    CommandLineParameterSource::~CommandLineParameterSource()
    {
        delete options;
    }

    void CommandLineParameterSource::parse(int argc, char** argv)
    {
        declareAcceptedOptions();
        options->processCommandArgs(argc, argv);
    }

	void CommandLineParameterSource::getHelp()
	{
		options->printHelp();
	}

    bool CommandLineParameterSource::parameterExists(std::string aName) const
    {
        return options->getValue(aName.c_str()) != nullptr;
    }

    bool CommandLineParameterSource::parameterExists(char c) const
    {

        return  options->getValue(c) != nullptr;
    }

    bool CommandLineParameterSource::findFlag(std::string aName) const
    {
        char* chars = (char*)malloc(sizeof(char) * aName.size() + 1);
        strcpy(chars, aName.c_str());
        return options->getFlag(chars);
    }

    std::string CommandLineParameterSource::getStringParameter(std::string aName) const
    {
        if(!parameterExists(aName))
            throw ParameterNotFoundException(aName);
        return std::string(options->getValue(aName.c_str()));
    }

    std::string CommandLineParameterSource::getStringParameter(char c) const
    {
        if (!parameterExists(c))
            throw ParameterNotFoundException(std::string(1, c));
        return std::string(options->getValue(c));
    }
}