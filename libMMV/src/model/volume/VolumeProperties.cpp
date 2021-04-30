#include "libmmv/model/volume/VolumeProperties.h"

namespace libmmv
{
    VolumeProperties::VolumeProperties(Voxel::DataType voxelType, const Vec3ui& resolution)
        : voxelType(voxelType)
        , resolution(resolution)
        , origin(Vec3f(0.0f,0.0f,0.0f))
        , boundingBox(Vec3f(resolution)*-0.5, Vec3f(resolution)*0.5)
    {
        init();
    }

    VolumeProperties::VolumeProperties(Voxel::DataType voxelType, const Vec3ui& resolution, const BoundingBox3f& bbox)
        : voxelType(voxelType)
        , resolution(resolution)
        , origin(bbox.getCenter())
        , boundingBox(bbox)
    {
        init();
    }

    VolumeProperties::VolumeProperties(Voxel::DataType voxelType, const Vec3ui& resolution, const Vec3f& voxelSize)
        : voxelType(voxelType)
        , resolution(resolution)
        , origin(Vec3f(0.0f, 0.0f, 0.0f))
        , boundingBox((Vec3f(resolution) * voxelSize)*-0.5, (Vec3f(resolution) * voxelSize)*0.5)
    {
        init();
    }

    VolumeProperties::~VolumeProperties()
    {
    }

    void VolumeProperties::init()
    {
        voxelCount = (size_t)resolution.x * (size_t)resolution.y * (size_t)resolution.z;
        sizeInBytes = getVolumeVoxelCount() * getVoxelByteWidth();
        voxelSize = boundingBox.getDimension() / (Vec3f)resolution;
        setSubVolumeCount(1U);
    }

    Voxel::DataType VolumeProperties::getVoxelType() const
    {
        return voxelType;
    }

    size_t VolumeProperties::getVoxelByteWidth() const
    {
        return Voxel::sizeOfVoxelType(voxelType);
    }

    const Vec3f& VolumeProperties::getVoxelSize() const
    {
        return voxelSize;
    }

    const Vec3ui& VolumeProperties::getVolumeResolution() const
    {
        return resolution;
    }

    const BoundingBox3f& VolumeProperties::getRealSpaceBoundingBox() const
    {
        return boundingBox;
    }

    const BoundingBox3i VolumeProperties::getMemoryBoundingBox() const
    {
        return BoundingBox3i(Vec3i(0, 0, 0), resolution - Vec3i(1, 1, 1));
    }

    size_t VolumeProperties::getVolumeVoxelCount() const
    {
        return voxelCount;
    }

    size_t VolumeProperties::getVolumeByteWidth() const
    {
        return sizeInBytes;
    }

    unsigned int VolumeProperties::getSubVolumeCount() const
    {
        return (unsigned int)subVolumeResolutions.size();
    }

    void VolumeProperties::setSubVolumeCount(unsigned int count)
    {
        if (count == VolumeProperties::SUBVOLUME_COUNT_AUTO) {
            count = resolution.z;
        }
        if (resolution.z < count)
        {
            std::stringstream message;
            message << "volume of size " << resolution << " cannot be split into " << count << " subvolumes";
            throw std::runtime_error( message.str() );
        }
        unsigned int subVolumeResZ = resolution.z / count;
        unsigned int remaining = resolution.z - count * subVolumeResZ;
        subVolumeResolutions.clear();
        subVolumeBaseCoords.clear();
        subVolumeBoundingBoxes.clear();
        auto nextMin = this->getRealSpaceBoundingBox().getMin();
        auto nextBaseCoordZ = 0U;
        for (unsigned int i = 0; i < count; ++i)
        {
            Vec3ui subBaseCoord(0, 0, nextBaseCoordZ);
            Vec3ui subRes(resolution.xy(), i < remaining ? subVolumeResZ + 1 : subVolumeResZ);
            BoundingBox3f subBoundingBox(nextMin, nextMin + (Vec3f)subRes * this->getVoxelSize());
            nextMin.z = subBoundingBox.getMax().z;
            nextBaseCoordZ += subRes.z;
            subVolumeResolutions.push_back(subRes);
            subVolumeBaseCoords.push_back(subBaseCoord);
            subVolumeBoundingBoxes.push_back(subBoundingBox);
        }
    }

    const Vec3ui& VolumeProperties::getSubVolumeResolution(unsigned int index) const
    {
        if (index < this->getSubVolumeCount())
        {
            return subVolumeResolutions[index];
        }
        else
        {
            throw std::runtime_error("tried to get resolution of subvolume " + std::to_string(index) + " while there are only " + std::to_string(this->getSubVolumeCount()));
        }
    }

    const Vec3ui& VolumeProperties::getSubVolumeBaseCoords(unsigned int index) const
    {
        if (index < this->getSubVolumeCount())
        {
            return subVolumeBaseCoords[index];
        }
        else
        {
            throw std::runtime_error("tried to get resolution of subvolume " + std::to_string(index) + " while there are only " + std::to_string(this->getSubVolumeCount()));
        }
    }

    const BoundingBox3f& VolumeProperties::getSubVolumeBoundingBox(unsigned int index) const
    {
        if (index < this->getSubVolumeCount())
        {
            return subVolumeBoundingBoxes[index];
        }
        else
        {
            throw std::runtime_error("Tried to get bounding box of subvolume " + std::to_string(index) + " while there are only " + std::to_string(this->getSubVolumeCount()));
        }
    }

    size_t VolumeProperties::getSubVolumeVoxelCount(unsigned int index) const
    {
        const Vec3ui& res = this->getSubVolumeResolution(index);
        return res.x * res.y * res.z;
    }

    size_t VolumeProperties::getSubVolumeByteWidth(unsigned int index) const
    {
        return this->getSubVolumeVoxelCount(index) * this->getVoxelByteWidth();
    }
}