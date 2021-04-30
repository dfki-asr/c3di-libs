#include <cstring>
#include "libmmv/model/volume/FloatVolume.h"
#include "libmmv/model/volume/Voxel.h"
#include "libmmv/model/image/Image.h"

namespace libmmv
{

    FloatVolume::FloatVolume(const Vec3ui& resolution, std::optional<float> initValue)
        : Volume(Voxel::DataType::FLOAT_32, resolution, initValue)
    {
        if (!initValue.has_value())
            Volume::init(0.0f);
        else
            Volume::init(*initValue);
    }

    FloatVolume::FloatVolume(const Vec3ui& resolution, const float* initialData)
        : Volume(Voxel::DataType::FLOAT_32, resolution, std::nullopt)
    {
        init(initialData);
    }

    FloatVolume::FloatVolume(const Vec3ui& resolution, const BoundingBox3f& bbox, std::optional<float> initValue)
        : Volume(Voxel::DataType::FLOAT_32, resolution, bbox, std::nullopt)
    {
        if (!initValue.has_value())
            Volume::init(0.0f);
        else
            Volume::init(*initValue);
    }

    FloatVolume::FloatVolume(const Vec3ui& resolution, const Vec3f& voxelSize, std::optional<float> initValue)
        : Volume(Voxel::DataType::FLOAT_32, resolution, voxelSize, initValue)
    {
        if (!initValue.has_value())
            Volume::init(0.0f);
        else
            Volume::init(*initValue);
    }

    FloatVolume::~FloatVolume()
    {
        delete data;
    }

    std::unique_ptr<float> FloatVolume::convertToFloat()
    {
        float* convertedData = new float[properties.getVolumeVoxelCount()];
        std::memcpy((void*)convertedData, (const void*)data, properties.getVolumeByteWidth());
        
        return std::unique_ptr<float>(convertedData);
    }

    void FloatVolume::setZSlice(unsigned int z, Image* image)
    {
        if(properties.getVolumeResolution().xy() != image->getResolution())
			throw std::range_error("Image resolution (" + image->getResolution().string() + " does not match volume z slice resolution (" + properties.getVolumeResolution().xy().string() + ")!");

        size_t sizeOfSlice = image->getResolution().x * image->getResolution().y;
        void* sliceStart = getRawPointerOfVoxel(Vec3ui(0, 0, z));

        std::memcpy(sliceStart, (void*)image->getData(), sizeOfSlice * properties.getVoxelByteWidth());
    }

    void FloatVolume::setVoxelToValueByCoord(const Vec3ui coords, float value)
    {
        setVoxelToValue(getVoxelIndex(coords), value);
    }

    void FloatVolume::setVoxelToValue(const size_t index, float value)
    {
        data[index] = value;
    }

    float FloatVolume::getVoxelValue(Vec3ui coords) const
    {
        return getVoxelValue(getVoxelIndex(coords));
    }

    float FloatVolume::getVoxelValue(size_t index) const
    {
        if( index == Volume::INVALID_VOXEL_INDEX )
            throw std::out_of_range("illegal voxel index");

        return data[index];
    }

    Image* FloatVolume::extractSlice(unsigned int index, CoordinateOrder orientation /*= ORDER_XYZ*/) const
    {
        if(orientation == ORDER_XYZ)
        {
            auto *result = new Image(properties.getVolumeResolution().xy());
            size_t sizeOfSlice = result->getResolution().x * result->getResolution().y;
            void* sliceStart = getRawPointerOfVoxel(Vec3ui(0, 0, index));
            std::memcpy((void*)result->getData(), sliceStart, sizeOfSlice * properties.getVoxelByteWidth());
            return result;
        } else {
            return Volume::extractSlice(index, orientation);
        }
    }

    void FloatVolume::init(const float* initialData)
    {
        allocateMemory();
        std::memcpy(data, initialData, properties.getVolumeByteWidth());
    }

    void FloatVolume::allocateMemory()
    {
        data = new float[properties.getVolumeVoxelCount()];
    }

    void FloatVolume::fillMemoryInNativeFormat(void* addr, float value, size_t count)
    {
        for( size_t i = 0; i < count; i++ )
        {
            ((float*)addr)[i] = value;
        }
    }

    float* FloatVolume::getFloatPointerStartingFrom(size_t offset /*= 0*/) const
    {
        return &data[offset];
    }

    void* FloatVolume::getRawPointerStartingFrom(size_t offset /*= 0*/) const
    {
        return &data[offset];
    }

}