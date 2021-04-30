#include "libmmv/io/datasource/ImageStackDirectoryDataSource.h"
#include <unordered_set>

#include "libmmv/io/deserializer/ImageDeserializer.h"
#include "libmmv/io/datasource/ImageStackDataSource.h"


namespace libmmv
{
    ImageStackDirectoryDataSource::ImageStackDirectoryDataSource()
    {
    }

    ImageStackDirectoryDataSource::~ImageStackDirectoryDataSource()
    {
    }

    Image* ImageStackDirectoryDataSource::loadProjectionImage(const HyperStackIndex& index)
    {
        Image* img = CachingImageStackDataSource::loadProjectionImage(index);
        if (img != nullptr) {
            return img;
        }

        auto findIt = imageLocations.find(index);
        if(findIt == imageLocations.end())
        {
            throw std::out_of_range( "there is no projection for index " + index.string() );
        }
        const ImageLocation& location = findIt->second;
        img = loadImageFromLocation(location);
        cacheProjectionImage(index, img);
        return img;
    }

    Image* ImageStackDirectoryDataSource::loadImageFromLocation(const ImageLocation& location) {
        Image* img = 0;
        std::filesystem::path fpath = getAbsoluteImageLocation(location.getPath());

        if (location.isInsideImageStack())
        {
            img = ImageDeserializer::readImageFromStack(fpath.string(), location.getIndexInImageStack());
        }
        else
        {
            img = ImageDeserializer::readImage(fpath.string());
        }
        return img;
    }

    std::filesystem::path ImageStackDirectoryDataSource::getAbsoluteImageLocation( const std::filesystem::path& location )
    {
        return directory / location.string();
    }

    std::vector<HyperStackIndex> ImageStackDirectoryDataSource::collectAllValidIndices() const
    {
        std::vector<HyperStackIndex> indices;
        for(auto it = imageLocations.begin(); it != imageLocations.end(); ++it)
        {
            indices.push_back(it->first);
        }
        return indices;
    }

    const char* ImageStackDirectoryDataSource::getName() const
    {
        return "ImageStackDirectoryDatasource";
    }

    libmmv::HyperStackIndex ImageStackDirectoryDataSource::firstIndex() const
    {
        return imageLocations.begin()->first;
    }

    libmmv::HyperStackIndex ImageStackDirectoryDataSource::lastIndex() const
    {
        return imageLocations.rbegin()->first;
    }

   
    Vec2ui ImageStackDirectoryDataSource::getResolution() const
    {
        return resolution;
    }

    unsigned int ImageStackDirectoryDataSource::getNumberOfProjections() const
    {
        return static_cast<unsigned int>(imageLocations.size());
    }

    //////////////////////////////////////////////////////////////////////////
    // ImageLocation
    //////////////////////////////////////////////////////////////////////////

    ImageStackDirectoryDataSource::ImageLocation::ImageLocation()
        : indexInImageStack(NOT_INSIDE_IMAGE_STACK)
    {
    }

    ImageStackDirectoryDataSource::ImageLocation::ImageLocation(const std::filesystem::path& path, unsigned int indexInImageStack)
        : path(path)
        , indexInImageStack(indexInImageStack)
    {
    }

    const std::filesystem::path& ImageStackDirectoryDataSource::ImageLocation::getPath() const
    {
        return path;
    }

    bool ImageStackDirectoryDataSource::ImageLocation::isInsideImageStack() const
    {
        return indexInImageStack != NOT_INSIDE_IMAGE_STACK;
    }

    unsigned int ImageStackDirectoryDataSource::ImageLocation::getIndexInImageStack() const
    {
        if(!this->isInsideImageStack())
        {
            throw std::ios_base::failure("path " + path.string() + " points directly to an image and not a stack of images");
        }
        return indexInImageStack;
    }
}