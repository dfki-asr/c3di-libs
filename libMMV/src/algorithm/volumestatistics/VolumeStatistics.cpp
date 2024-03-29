#include "libmmv/algorithm/volumestatistics/VolumeStatistics.h"
#include "libmmv/model/volume/FloatVolume.h"
#include <limits>
namespace libmmv
{
    VolumeStatistics::VolumeStatistics()
        : min(std::numeric_limits<float>::quiet_NaN())
        , max(std::numeric_limits<float>::quiet_NaN())
        , mean(std::numeric_limits<float>::quiet_NaN())
    {
    }

    VolumeStatistics::VolumeStatistics(float min, float max, float mean)
        : min(min), max(max), mean(mean)
    {
    }

    libmmv::VolumeStatistics VolumeStatistics::compute(Volume* volume)
    {
        float min = std::numeric_limits<float>::max();
        float max = std::numeric_limits<float>::min();
        float mean = 0.0f;
        unsigned int numberOfVoxels = 0;

        VolumeStatistics stat(min, max, mean);

        size_t test = volume->getProperties().getVolumeVoxelCount();
        for(size_t i = 0; i < volume->getProperties().getVolumeVoxelCount(); i++)
        {
            float voxelValue = volume->getVoxelValue(i);

            if (!std::isnan(voxelValue))
            {
                if (voxelValue < min)
                    min = voxelValue;

                if (voxelValue > max)
                    max = voxelValue;

                mean += voxelValue;
                numberOfVoxels++;
            }
        }

        float scalingFactor = (1 / (float)volume->getProperties().getVolumeVoxelCount());

        mean *= scalingFactor;

        stat.min = min;
        stat.max = max;
        stat.mean = mean;

        return stat;
    }

    VolumeStatistics& VolumeStatistics::operator=(const VolumeStatistics& other)
    {
        min = other.min;
        max = other.max;
        mean = other.mean;
        return *this;
    }

    float VolumeStatistics::getMin() const
    {
        return this->min;
    }

    float VolumeStatistics::getMax() const
    {
        return this->max;
    }

    float VolumeStatistics::getMean() const
    {
        return this->mean;
    }
}