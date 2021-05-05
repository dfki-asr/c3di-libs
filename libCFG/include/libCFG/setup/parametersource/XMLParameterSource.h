#pragma once

#include <rapidxml.hpp>

#include "libCFG/setup/parametersource/ParameterSource.h"

namespace libCFG
{
    class XMLParameterSource : public ParameterSource
    {
    public:
        XMLParameterSource(const std::string filename);
        virtual ~XMLParameterSource();

        virtual bool parameterExists(std::string nodeName) const override;
        virtual std::string getStringParameter(std::string nodeName) const override;
        virtual std::filesystem::path getPathParameter(std::string nodeName) const override;
        virtual void parse(int argc = 0, char** argv = nullptr) override;
    protected:
        std::filesystem::path extendRelativeToXMLPath(std::filesystem::path path) const;

        std::string xmlFilename;
        rapidxml::xml_document<> xml_document;
        //The string must persist for the lifetime of the xml_document. from http://rapidxml.sourceforge.net/manual.html
        std::string file_content;
    };
}
