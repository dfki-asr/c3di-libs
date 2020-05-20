#include "stdafx.h"

#include "ParameterNotFoundException.h"

namespace ettention
{
    ParameterNotFoundException::ParameterNotFoundException(std::string parameter)
        : std::runtime_error(std::string("parameter not found: ") + parameter), parameter(parameter)
    {
    }

    std::string ParameterNotFoundException::getParameter()
    {
        return parameter;
    }
}