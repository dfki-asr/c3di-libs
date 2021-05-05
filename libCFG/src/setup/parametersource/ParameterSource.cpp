#include "libCFG/setup/parametersource/ParameterSource.h"

namespace libCFG
{
    ParameterSource::ParameterSource()
        : logLevel(ParameterSource::LogLevel::SILENT)
    {
    }

    ParameterSource::~ParameterSource()
    {
    }

    ParameterSource::LogLevel ParameterSource::getLogLevel() const
    {
        return logLevel;
    }

    void ParameterSource::setLogLevel(const LogLevel level)
    {
        this->logLevel = level;
    }

    std::filesystem::path ParameterSource::getPathParameter(std::string aName) const
    {
        std::string strValue = getStringParameter(aName);
        return std::filesystem::path(strValue);
    }

    float ParameterSource::getFloatParameter(std::string aName) const
    {
        std::string strValue = getStringParameter(aName);
        return std::stof(strValue);
    }

    bool ParameterSource::getBoolParameter(std::string aName) const
    {
        std::string strValue = getStringParameter(aName);
        std::transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
        if (strValue == "true")
            return true;
        if (strValue == "false")
            return false;
        throw std::runtime_error("illegal parameter format, expect true/false");
    }

    int ParameterSource::getIntParameter(std::string aName) const
    {
        std::string strValue = getStringParameter(aName);
        return std::stoi(strValue);
    }

    unsigned int ParameterSource::getUIntParameter(std::string aName) const
    {
        std::string strValue = getStringParameter(aName);
        return std::stoul(strValue);
    }

    libmmv::Vec2ui ParameterSource::getVec2uiParameter(std::string aName) const
    {
        return getVectorParameter<libmmv::Vec2ui>( aName );
    }

    libmmv::Vec3ui ParameterSource::getVec3uiParameter(std::string aName) const
    {
        return getVectorParameter<libmmv::Vec3ui>(aName);
    }

    libmmv::Vec3f ParameterSource::getVec3fParameter(std::string aName) const
    {
        return getVectorParameter<libmmv::Vec3f>(aName);
    }

    libmmv::Vec2f ParameterSource::getVec2fParameter(std::string aName) const
    {
        return getVectorParameter<libmmv::Vec2f>(aName);
    }

    std::string ParameterSource::replaceCommasWithSpaces(std::string str) const
    {
        std::replace(str.begin(), str.end(), ',', ' ');
        return str;
    }


    template<class TVECTOR>
    TVECTOR ParameterSource::getVectorParameter(std::string aName) const
    {
        std::string buffer = replaceCommasWithSpaces(getStringParameter(aName));
        std::istringstream stream(buffer);
        TVECTOR result;
        stream >> result;
        return result;
    }

}