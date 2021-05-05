#include "libCFG/setup/parametersource/XMLParameterSource.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "libmmv/algorithm/FileAlgorithms.h"
#include "libmmv/algorithm/VectorAlgorithms.h"

namespace libCFG
{

    XMLParameterSource::XMLParameterSource(const std::string filename)
        : xmlFilename(filename)
    {
        parse();
        setLogLevel(ParameterSource::LogLevel::OVERALL_PROGRESS);
    }

    XMLParameterSource::~XMLParameterSource()
    {
    }

    std::filesystem::path XMLParameterSource::getPathParameter(std::string aName) const
    {
        std::string xmlValue = getStringParameter(aName);
        std::replace(xmlValue.begin(), xmlValue.end(), '\\', '/');
        std::filesystem::path path = std::filesystem::path(xmlValue);
        if(path.is_relative())
        {
            path = extendRelativeToXMLPath(path);
        }
        return path;
    }

    bool XMLParameterSource::parameterExists(std::string parameterName) const
    {
        auto property_path = libmmv::VectorAlgorithms::splitString(parameterName, ".");
        std::string property = property_path[0];
        rapidxml::xml_node<>* node = xml_document.first_node(property.c_str());
        if (node == nullptr)
            return false;
        for (size_t i = 1; i < property_path.size(); i++)
        {
            std::string property = property_path[i];
            node = node->first_node( &property[0] );
            if (node == nullptr)
                return false;
        }
        return true;
    }

    std::string XMLParameterSource::getStringParameter(std::string parameterName)  const
    {
        auto property_path = libmmv::VectorAlgorithms::splitString(parameterName, ".");
        std::string property = property_path[0];
        rapidxml::xml_node<>* node = xml_document.first_node( &property[0] );
        if ( node == nullptr )
            throw std::runtime_error(parameterName);
        for ( size_t i = 1; i < property_path.size(); i++ )
        {
            std::string property = property_path[i];
            node = node->first_node( &property[0] );
            if (node == nullptr)
                throw std::runtime_error(parameterName);
        }
        return node->first_attribute("value")->value();
    };

    void XMLParameterSource::parse(int argc, char** argv)
    {
        std::ifstream ifs(xmlFilename);
        file_content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        xml_document.parse<0>(&file_content[0]);
    }

    std::filesystem::path XMLParameterSource::extendRelativeToXMLPath(std::filesystem::path path) const
    {
        std::filesystem::path pathToXMLFile = std::filesystem::path(xmlFilename).parent_path();
        std::filesystem::path totalPath = pathToXMLFile / path;
        return libmmv::FileAlgorithms::normalizePath(totalPath.string());
    }

}