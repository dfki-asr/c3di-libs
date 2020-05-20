#include "stdafx.h"

#include "VolumeDeserializer.h"
#include "model/volume/ByteVolume.h"
#include "model/volume/HalfFloatVolume.h"
#include "model/volume/FloatVolume.h"
#include "model/volume/VolumeProperties.h"
#include "io/datasource/MRCDataSource.h"
#include "io/datasource/ImageStackDataSource.h"

namespace ettention
{
    Volume* VolumeDeserializer::load(ImageStackDataSource* dataSource, Voxel::DataType voxelType, unsigned int subVolumeCount)
    {
        Vec3ui volumeResolution = Vec3ui(dataSource->getResolution(), dataSource->getNumberOfProjections());

        Volume *volume;
        switch( voxelType )
        {
        case ettention::Voxel::DataType::UCHAR_8:
            volume = new ByteVolume(volumeResolution, 0.0f);
            break;
        case ettention::Voxel::DataType::HALF_FLOAT_16:
            volume = new HalfFloatVolume(volumeResolution, 0.0f);
            break;
        case ettention::Voxel::DataType::FLOAT_32:
            volume = new FloatVolume(volumeResolution, 0.0f);
            break;
        default:
            throw std::invalid_argument("invalid voxel type passed to volume deserializer");
        }

        for(unsigned int z = 0; z < volumeResolution.z; z++)
        {
            volume->setZSlice(z, dataSource->loadProjectionImage(z));
        }
        return volume;
    }

    Volume* VolumeDeserializer::load(std::string volumeFileName, Voxel::DataType voxelType, unsigned int subVolumeCount)
    {
        MRCDataSource* dataSource = new MRCDataSource(volumeFileName, false);
        auto volume = load(dataSource, voxelType, subVolumeCount);
        delete dataSource;
        return volume;
    }
}
