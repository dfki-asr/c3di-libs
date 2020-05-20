#include "stdafx.h"
#include "CachingImageStackDataSource.h"

#include "model/image/Image.h"
#include <cstdio>

namespace ettention
{
    CachingImageStackDataSource::CachingImageStackDataSource(unsigned int maxCachedImages)
        : maxCachedImages(maxCachedImages)
    {
        if(maxCachedImages == 0)
        {
            throw std::runtime_error("Maximum cached images for CachingImageStackDataSource must be at least 1!");
        }
    }

    CachingImageStackDataSource::~CachingImageStackDataSource()
    {
        clearCachedProjectionImages();
    }

    void CachingImageStackDataSource::setMaxCachedImages(unsigned int max) {
        maxCachedImages = max;
        this->reorganizeCache();
    }

    Image* CachingImageStackDataSource::loadProjectionImage(const HyperStackIndex& index)
    {
        auto findIt = projectionImages.find(index);
        if (findIt != projectionImages.end())
            return findIt->second;
        return nullptr;
    }

    void CachingImageStackDataSource::cacheProjectionImage(const HyperStackIndex& index, Image* img) {
        projectionImages[index] = img;
        lruProjectionImages.push_front(std::make_pair(index, img));
        this->reorganizeCache();
    }

    void CachingImageStackDataSource::clearCachedProjectionImages()
    {
        for(auto it = projectionImages.begin(); it != projectionImages.end(); ++it)
        {
           delete it->second;
        }
        projectionImages.clear();
        lruProjectionImages.clear();
    }

    void CachingImageStackDataSource::reorganizeCache()
    {
        while(lruProjectionImages.size() > maxCachedImages)
        {
            auto entry = lruProjectionImages.back();
            lruProjectionImages.pop_back();
            delete entry.second;
            projectionImages.erase(projectionImages.find(entry.first));
        }
    }
}