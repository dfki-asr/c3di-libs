#include "libCFG/error/ParameterNotFoundException.h"

namespace libCFG
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