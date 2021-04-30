#pragma once

namespace libmmv
{

    class FileAlgorithms
    {
    public:
        static std::filesystem::path normalizePath(const std::filesystem::path &path);
    };
}