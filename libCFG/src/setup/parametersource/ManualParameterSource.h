#pragma once

#include "setup/parametersource/ParameterSource.h"

#include <map>

namespace ettention
{
    class ManualParameterSource : public ParameterSource
    {
    public:
        ManualParameterSource();
        ~ManualParameterSource();

        void setParameter(std::string name, std::string value, std::string type );

        virtual bool parameterExists(std::string aName) const override;
        virtual std::string getStringParameter(std::string aName) const override;
 
    private:
        std::map<std::string, std::pair<std::string, std::string> > parameter;
    };

}

