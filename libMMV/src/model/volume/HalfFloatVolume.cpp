#include "libmmv/model/volume/HalfFloatVolume.h"
#include "libmmv/model/volume/Voxel.h"
#include "libmmv/math/Float16Compressor.h"
#include "libmmv/model/image/Image.h"

namespace libmmv
{

    HalfFloatVolume::HalfFloatVolume(const Vec3ui& resolution, std::optional<float> initValue)
        : Volume(Voxel::DataType::HALF_FLOAT_16, resolution, initValue)
    {
        Volume::init(initValue ? 0.0f : *initValue);
    }

    HalfFloatVolume::HalfFloatVolume(const Vec3ui& resolution, const BoundingBox3f& bbox, std::optional<float> initValue)
        : Volume(Voxel::DataType::HALF_FLOAT_16, resolution, bbox, std::nullopt)
    {
        Volume::init(initValue ? 0.0f : *initValue);
    }

    HalfFloatVolume::HalfFloatVolume(const Vec3ui& resolution, const float* initialData)
        : Volume(Voxel::DataType::HALF_FLOAT_16, resolution, std::nullopt)
    {
        init(initialData);
    }

    HalfFloatVolume::~HalfFloatVolume()
    {
        delete data;
    }

    std::unique_ptr<float> HalfFloatVolume::convertToFloat()
    {
        float* convertedData = new float[properties.getVolumeVoxelCount()];
        Float16Compressor::decompress(data, convertedData, properties.getVolumeVoxelCount());
        
        return std::unique_ptr<float>(convertedData);
    }

    void HalfFloatVolume::setZSlice(unsigned int z, Image* image)
    {
        if(properties.getVolumeResolution().xy() != image->getResolution())
			throw std::runtime_error("Image resolution (" + image->getResolution().string() + " does not match volume z slice resolution (" + properties.getVolumeResolution().xy().string() + ")!");

        size_t sizeOfSlice = image->getResolution().x * image->getResolution().y;
        void* sliceStart = getRawPointerOfVoxel(Vec3ui(0, 0, z));

        Float16Compressor::compress(image->getData(), (half_float_t*)sliceStart, sizeOfSlice);
    }

    void HalfFloatVolume::setVoxelToValue(const size_t index, float value)
    {
        data[index] = Float16Compressor::compress(value);
    }

    float HalfFloatVolume::getVoxelValue(size_t index) const
    {
        if( index == Volume::INVALID_VOXEL_INDEX )
            throw std::out_of_range("illegal voxel index");

        return Float16Compressor::decompress(data[index]);
    }

    void HalfFloatVolume::allocateMemory()
    {
        data = new half_float_t[properties.getVolumeVoxelCount()];
    }

    void HalfFloatVolume::fillMemoryInNativeFormat(void* addr, float value, size_t count)
    {
        auto halfValue = Float16Compressor::compress(value);
        for( size_t i = 0; i < count; i++ )
        {
            ((half_float_t*)addr)[i] = halfValue;
        }
    }

    void* HalfFloatVolume::getRawPointerStartingFrom(size_t offset /*= 0*/) const
    {
        return &data[offset];
    }

    void HalfFloatVolume::init(const float* initialData)
    {
        allocateMemory();
        Float16Compressor::compress(initialData, data, properties.getVolumeVoxelCount());
    }

}