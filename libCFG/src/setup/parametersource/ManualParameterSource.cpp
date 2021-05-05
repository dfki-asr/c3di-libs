#include "libCFG/setup/parametersource/ManualParameterSource.h"

namespace libCFG
{

    ManualParameterSource::ManualParameterSource()
    {
    }

    ManualParameterSource::~ManualParameterSource()
    {
    }

	void ManualParameterSource::parse(int argc, char** argv)
	{
    }

    void ManualParameterSource::setParameter(std::string name, std::string value, std::string type)
    {
        parameter[name] = std::pair( value, type );
    }

    bool ManualParameterSource::parameterExists(std::string aName) const
    {
        auto paramIterator = parameter.find(aName);
        return paramIterator != parameter.end();
    }

    std::string ManualParameterSource::getStringParameter(std::string aName) const
    {
        auto iterator = parameter.find(aName);
        if (iterator == parameter.end() )
            throw std::runtime_error("no such parameter exception");
        std::pair<std::string, std::string> vlaueTypePair = iterator->second;
        std::string parameter = vlaueTypePair.first;
        return parameter;
    }
}