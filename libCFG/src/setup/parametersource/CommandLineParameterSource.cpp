#include "CommandLineParameterSource.h"

#include "anyoption.h"

#include "error/ParameterNotFoundException.h"

namespace ettention
{
    CommandLineParameterSource::CommandLineParameterSource(int argc, char** argv)
        : argc(argc), argv(argv)
    {
        options = new AnyOption();
    }

    CommandLineParameterSource::~CommandLineParameterSource()
    {
        delete options;
    }

    void CommandLineParameterSource::parse()
    {
        declareAcceptedOptions();
        options->processCommandArgs(argc, argv);
    }

    bool CommandLineParameterSource::parameterExists(std::string aName) const
    {
        return options->getValue(aName.c_str()) != nullptr;
    };

    std::string CommandLineParameterSource::getStringParameter(std::string aName) const
    {
        if(!parameterExists(aName))
            throw ParameterNotFoundException(aName);
        return std::string( options->getValue( aName.c_str() ) );
    };

}