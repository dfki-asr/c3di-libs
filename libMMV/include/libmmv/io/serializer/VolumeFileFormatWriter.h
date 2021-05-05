#pragma once
#include <stdio.h>
#include <filesystem>

#include "libmmv/io/IOEnumerations.h"

namespace libCFG
{
    class OutputParameterSet;
}

namespace libmmv
{
    class RangeTransformation;
    class VolumeProperties;
    class Volume;
    class Image;

    class VolumeFileFormatWriter
    {
    public:

        virtual bool canGenerateVoxelType(VoxelValueType mode) = 0;
        virtual bool canGenerateRotation(CoordinateOrder rotation) = 0;
        virtual std::string getFileExtension() = 0;

        virtual void write(Volume* volume, std::filesystem::path outputVolumeFileName, VoxelValueType mode, CoordinateOrder order, bool invert = false);


    protected:
        VoxelValueType currentVoxelMode;
        CoordinateOrder currentOrientation;

        virtual void writeHeader(std::ofstream &outfile, VolumeProperties& volumeProperties, const Vec3ui volumeResolution, RangeTransformation* rangeTransformation) = 0;

        RangeTransformation* createRangeTransformation(Volume* volume, bool invert);
        void writePixelwise(std::ofstream &outfile, Vec3ui &outputVolumeResolution, Volume* volume, std::unique_ptr<float> &volumeDataInFloatFormat);
        void writeSlicewise(std::ofstream &output, Vec3ui &outputVolumeResolution, Volume* volume);
        void writeValue(std::ofstream &output, float value);
        void writeSlice(std::ofstream &output, Image *image);
    };
}
