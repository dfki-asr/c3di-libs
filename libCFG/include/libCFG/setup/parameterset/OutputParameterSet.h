#pragma once

#include <filesystem>

#include "libCFG/setup/parameterset/ParameterSet.h"
#include "libmmv/io/IOEnumerations.h"
#include "libmmv/io/serializer/VolumeSerializer.h"

namespace libCFG
{
    class ParameterSource;

    class OutputParameterSet : public ParameterSet
    {
    public:
        OutputParameterSet(libmmv::VoxelValueType voxelValueType = libmmv::IO_VOXEL_TYPE_FLOAT_32, libmmv::CoordinateOrder orientation = libmmv::ORDER_XYZ, bool invert = false);
        OutputParameterSet(const ParameterSource* parameterSource);
        virtual ~OutputParameterSet();

        const std::filesystem::path& getFilename() const;
        const std::string& getFormat() const;
        libmmv::VoxelValueType getVoxelType() const;
        libmmv::CoordinateOrder getOrientation() const;
        bool getInvert() const;
        bool getEnableVoxelization() const;

        void testValidityOfOutputOptions(libmmv::VolumeSerializer *serializer) const;

    protected:
        void testAvailabilityOfOutputDirectory(std::filesystem::path filename);

    private:
        std::filesystem::path filename;
        std::string format;
        libmmv::VoxelValueType voxelType;
        libmmv::CoordinateOrder orientation;
        bool invert;
        bool enableVoxelization;
    };
}