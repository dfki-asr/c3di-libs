#include "libmmv/io/deserializer/VolumeDeserializer.h"
#include "libmmv/model/volume/ByteVolume.h"
#include "libmmv/model/volume/HalfFloatVolume.h"
#include "libmmv/model/volume/FloatVolume.h"
#include "libmmv/model/volume/VolumeProperties.h"
#include "libmmv/io/datasource/MRCDataSource.h"
#include "libmmv/io/datasource/ImageStackDataSource.h"

namespace libmmv
{
    Volume* VolumeDeserializer::load(ImageStackDataSource* dataSource, Voxel::DataType voxelType, unsigned int subVolumeCount)
    {
        Vec3ui volumeResolution = Vec3ui(dataSource->getResolution(), dataSource->getNumberOfProjections());

        Volume *volume;
        switch( voxelType )
        {
        case libmmv::Voxel::DataType::UCHAR_8:
            volume = new ByteVolume(volumeResolution, 0.0f);
            break;
        case libmmv::Voxel::DataType::HALF_FLOAT_16:
            volume = new HalfFloatVolume(volumeResolution, 0.0f);
            break;
        case libmmv::Voxel::DataType::FLOAT_32:
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
