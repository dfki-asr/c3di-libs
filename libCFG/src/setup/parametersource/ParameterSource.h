#pragma once

#include <filesystem>

#include "math/Vec2.h"
#include "math/Vec3.h"

namespace ettention
{
    class ParameterSource
    {
    public:
        enum class LogLevel : int
        {
            SILENT,
            OVERALL_PROGRESS,
            PARAMETER_DETAILS,
            ALL
        };

        ParameterSource();
        virtual ~ParameterSource();

        virtual LogLevel getLogLevel() const;
        virtual void setLogLevel(const LogLevel level);

        virtual void parse() = 0;

        virtual bool parameterExists(std::string aName) const = 0;

        virtual std::string getStringParameter(std::string aName) const = 0;
        virtual std::filesystem::path getPathParameter(std::string aName) const;

        virtual float getFloatParameter(std::string aName) const;
        virtual bool getBoolParameter(std::string aName) const;
        virtual int getIntParameter(std::string aName) const;
        virtual unsigned int getUIntParameter(std::string aName) const;
        virtual Vec2ui getVec2uiParameter(std::string aName) const;
        virtual Vec3ui getVec3uiParameter(std::string aName) const;
        virtual Vec3f getVec3fParameter(std::string aName) const;
        virtual Vec2f getVec2fParameter(std::string aName) const;

    protected:
        std::string replaceCommasWithSpaces(std::string str) const;
        ParameterSource::LogLevel logLevel;

        template<class TVECTOR>
        TVECTOR getVectorParameter(std::string aName) const;
    };
}