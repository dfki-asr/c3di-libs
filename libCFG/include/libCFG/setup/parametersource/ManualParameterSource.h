#pragma once

#include "libCFG/setup/parametersource/ParameterSource.h"
#include <map>

namespace libCFG
{
    class ManualParameterSource : public ParameterSource
    {
    public:
        ManualParameterSource();
        ~ManualParameterSource();

        void setParameter(std::string name, std::string value, std::string type );
        virtual void parse(int argc = 0, char** argv = nullptr) override;
        virtual bool parameterExists(std::string aName) const override;
        virtual std::string getStringParameter(std::string aName) const override;
 
    private:
        std::map<std::string, std::pair<std::string, std::string> > parameter;
    };

}

