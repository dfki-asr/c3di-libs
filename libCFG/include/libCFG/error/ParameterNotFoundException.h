#pragma once

#include  <stdexcept>

namespace libCFG
{
    class ParameterNotFoundException : public std::runtime_error
    {
    public:
        ParameterNotFoundException(std::string message);
        std::string getParameter();

    protected:
        std::string parameter;
    };
}
