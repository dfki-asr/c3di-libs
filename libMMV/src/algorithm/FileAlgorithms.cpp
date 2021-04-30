#include <stdexcept>
#include <filesystem>
#include "libmmv/algorithm/FileAlgorithms.h"

namespace libmmv
{
    std::filesystem::path FileAlgorithms::normalizePath(const std::filesystem::path &path)
    {
        std::filesystem::path absPath = std::filesystem::absolute(path);
        std::filesystem::path::iterator it = absPath.begin();
        std::filesystem::path result = *it++;

        // Get canonical version of the existing part
        for(; it != absPath.end() && exists(result / *it); ++it)
        {
            result /= *it;
        }
        result = canonical(result);

        // For the rest remove ".." and "." in a path with no symlinks
        for(; it != absPath.end(); ++it)
        {
            // Just move back on ../
            if(*it == "..")
            {
                result = result.parent_path();
            }
            // Ignore "."
            else if(*it != ".")
            {
                // Just cat other path entries
                result /= *it;
            }
        }

        return result;
    }
}