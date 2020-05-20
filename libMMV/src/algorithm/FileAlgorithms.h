#pragma once

namespace ettention
{

    class FileAlgorithms
    {
    public:
        static std::filesystem::path normalizePath(const std::filesystem::path &path);
    };
}