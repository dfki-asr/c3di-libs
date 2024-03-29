#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "libmmv/model/volume/Volume.h"

namespace libmmv
{
    class ImageStackDataSource;

    /*
    The class reads entire volumes from disc. You can either pass a filename to an volume file in mrc format, an std::string or an ImageStackDataSource. In either case,
    the volume is read entirely to memory. 
    */
    class VolumeDeserializer
    {
    public:
        static Volume* load(std::string fileName, Voxel::DataType voxelType, unsigned int subVolumeCount = 1U);
        static Volume* load(ImageStackDataSource* dataSource, Voxel::DataType voxelType, unsigned int subVolumeCount = 1U);
    };   
}
