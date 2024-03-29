#pragma once

namespace libmmv
{
    class Volume;

    class VolumeStatistics
    {
    public:
        VolumeStatistics();
        VolumeStatistics(float min, float max, float mean);

        static VolumeStatistics compute(Volume* volume);
        VolumeStatistics& operator=(const VolumeStatistics& other);

        float getMin() const;
        float getMax() const;
        float getMean() const;

    protected:
        float min, max, mean;
    };
}

