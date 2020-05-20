#pragma once

#include <rapidxml.hpp>

#include "setup/parametersource/ParameterSource.h"

namespace ettention
{
    class XMLParameterSource : public ParameterSource
    {
    public:
        XMLParameterSource(const std::string filename);
        virtual ~XMLParameterSource();

        virtual bool parameterExists(std::string nodeName) const override;
        virtual std::string getStringParameter(std::string nodeName) const override;
        virtual std::filesystem::path getPathParameter(std::string nodeName) const override;

    protected:
        virtual void parse() override;
        std::filesystem::path extendRelativeToXMLPath(std::filesystem::path path) const;

        std::string xmlFilename;
        rapidxml::xml_document<> xml_document;
    };
}
