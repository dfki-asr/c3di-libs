#pragma once

#include <vector>

namespace ettention
{
    class VectorAlgorithms
    {
    public:
        static void splitString(const std::string& textToSplit, const std::string& delimiter, std::vector<std::string>& result);
        static std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);
    };

}