

#include "libmmv/algorithm/VectorAlgorithms.h"

#include <regex>
#include <sstream> 

namespace libmmv
{
    void VectorAlgorithms::splitString(const std::string& textToSplit, const std::string& delimiter, std::vector<std::string>& result)
    {
        size_t start = 0;
        size_t end = 0;

        while(end != std::string::npos)
        {
            end = textToSplit.find(delimiter, start);
            std::string token = textToSplit.substr(start, (end == std::string::npos) ? std::string::npos : end - start);
            if(token != "")
                result.push_back(token);
            start = ((end > (std::string::npos - delimiter.size())) ? std::string::npos : end + delimiter.size());
        }
    }

    std::vector<std::string> VectorAlgorithms::splitString(const std::string& str, const std::string& delimiter)
    {
        std::vector<std::string> result;
        splitString(str, delimiter, result);
        return result;
    }
}