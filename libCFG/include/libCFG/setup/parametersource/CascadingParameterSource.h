#pragma once

#include <list>

#include "libCFG/setup/parametersource/ParameterSource.h"

namespace libCFG
{
    class CascadingParameterSource : public ParameterSource
    {
    private:
        std::list<ParameterSource*> sources;

    public:
        CascadingParameterSource();
        virtual ~CascadingParameterSource();

        virtual bool parameterExists(std::string aName) const override;
        virtual std::filesystem::path getPathParameter(std::string aName) const override;
        virtual std::string getStringParameter(std::string aName) const override;
        virtual float getFloatParameter(std::string aName) const override;
        virtual bool getBoolParameter(std::string aName) const override;
        virtual int getIntParameter(std::string aName) const override;
        virtual unsigned int getUIntParameter(std::string aName) const override;
        virtual libmmv::Vec3ui getVec3uiParameter(std::string aName) const override;
        virtual libmmv::Vec3f getVec3fParameter(std::string aName) const override;

        virtual void parse(int argc = 0, char** argv = nullptr) override;
        virtual void parseAll(int argc, char** argv);
        virtual void addSource(ParameterSource* source);
        virtual void parseAndAddSource(ParameterSource* source);

        virtual ParameterSource::LogLevel getLogLevel() const override;
        virtual void setLogLevel(const ParameterSource::LogLevel level) override;
    };
}
