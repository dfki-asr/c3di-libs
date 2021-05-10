#pragma once

#include "libCFG/setup/parametersource/ParameterSource.h"
#include <vector>
#include <filesystem>

class AnyOption;

namespace libCFG
{  
    class CommandLineParameterSource : public ParameterSource
    {
    public:
        CommandLineParameterSource();
        CommandLineParameterSource(int argc, char** argv);
        virtual ~CommandLineParameterSource();

        virtual void parse(int argc = 0, char** argv = nullptr) override;
        virtual void declareAcceptedOptions() = 0;
		void getHelp();

        virtual bool parameterExists(std::string aName) const override;
        virtual bool parameterExists(char c) const override;
        virtual bool findFlag(std::string aName) const;
        virtual std::string getStringParameter(std::string aName) const override;
        virtual std::string getStringParameter(char c) const override;

    protected:
        AnyOption* options;
        int argc;
        char** argv;
    };
}
