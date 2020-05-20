#pragma once

#include  <stdexcept>

namespace ettention
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
