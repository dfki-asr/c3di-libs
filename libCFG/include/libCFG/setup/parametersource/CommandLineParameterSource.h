#pragma once

#include "libCFG/setup/parametersource/ParameterSource.h"

class AnyOption;

namespace libCFG
{  
    class CommandLineParameterSource : public ParameterSource
    {
    public:
        CommandLineParameterSource(int argc, char** argv);
        virtual ~CommandLineParameterSource();

        virtual void parse() override;
        virtual void declareAcceptedOptions() = 0;

        virtual bool parameterExists(std::string aName) const override;
        virtual std::string getStringParameter(std::string aName) const override;

    protected:
        AnyOption* options;
        int argc;
        char** argv;
    };
}
