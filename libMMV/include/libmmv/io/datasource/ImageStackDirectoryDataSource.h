#pragma once
#include <vector>
#include <map>
#include <filesystem>
#include "libmmv/io/datasource/CachingImageStackDataSource.h"

namespace libmmv
{
    class ImageStackDirectoryDataSource : public CachingImageStackDataSource
    {
    protected:
        class ImageLocation
        {
        protected:
            std::filesystem::path path;
            unsigned int indexInImageStack;

        public:
            static const unsigned int NOT_INSIDE_IMAGE_STACK = (unsigned int)-1;

            ImageLocation();
            ImageLocation(const std::filesystem::path& path, unsigned int indexInImageStack = NOT_INSIDE_IMAGE_STACK);

            const std::filesystem::path& getPath() const;
            bool isInsideImageStack() const;
            unsigned int getIndexInImageStack() const;
        };

    public:
        ImageStackDirectoryDataSource();
        ~ImageStackDirectoryDataSource();

        std::vector<HyperStackIndex> collectAllValidIndices() const;

        Vec2ui getResolution() const override;
        const char* getName() const override;
        unsigned int getNumberOfProjections() const override;
        Image* loadProjectionImage(const HyperStackIndex& index) override;
        virtual HyperStackIndex firstIndex() const override;
        virtual HyperStackIndex lastIndex() const override;

    protected:
        std::filesystem::path directory;
        Vec2ui resolution;
        std::map<HyperStackIndex, ImageLocation> imageLocations;

        std::filesystem::path getAbsoluteImageLocation(const std::filesystem::path& location);
        Image* loadImageFromLocation(const ImageLocation& location);
    };
}