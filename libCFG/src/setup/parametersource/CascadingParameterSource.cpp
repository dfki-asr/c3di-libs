#include "libCFG/setup/parametersource/CascadingParameterSource.h"
#include "libCFG/error/ParameterNotFoundException.h"

namespace libCFG
{
    CascadingParameterSource::CascadingParameterSource()
    {
    }

    CascadingParameterSource::~CascadingParameterSource()
    {
    }

    bool CascadingParameterSource::parameterExists(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return true;
            }
        }
        return false;
    };

    std::filesystem::path CascadingParameterSource::getPathParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getPathParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    };

    std::string CascadingParameterSource::getStringParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getStringParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    };

    float CascadingParameterSource::getFloatParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getFloatParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    }

    bool CascadingParameterSource::getBoolParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getBoolParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    }

    int CascadingParameterSource::getIntParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getIntParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    }

    unsigned int CascadingParameterSource::getUIntParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getUIntParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    }

    libmmv::Vec3ui CascadingParameterSource::getVec3uiParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getVec3uiParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    }

    libmmv::Vec3f CascadingParameterSource::getVec3fParameter(std::string aName) const
    {
        for(auto it = sources.begin(); it != sources.end(); ++it)
        {
            if((*it)->parameterExists(aName))
            {
                return (*it)->getVec3fParameter(aName);
            }
        }
        throw ParameterNotFoundException(aName);
    }

    void CascadingParameterSource::parse(int argc, char** argv)
    {
        parseAll(argc, argv);
    }

    void CascadingParameterSource::parseAll(int argc, char** argv)
    {
        for( auto it = sources.begin(); it != sources.end(); ++it )
        {
            (*it)->parse(argc, argv);
        }
    }

    void CascadingParameterSource::addSource(ParameterSource* source)
    {
        sources.push_front(source);
    }

    void CascadingParameterSource::parseAndAddSource(ParameterSource* source)
    {
        source->parse();
        addSource(source);
    }

    ParameterSource::LogLevel CascadingParameterSource::getLogLevel() const
    {
        return ParameterSource::LogLevel::OVERALL_PROGRESS;
    }

    void CascadingParameterSource::setLogLevel(const ParameterSource::LogLevel level)
    {
        //
    }
}