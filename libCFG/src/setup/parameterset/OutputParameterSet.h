#pragma once

#include <filesystem>

#include "setup/parameterset/ParameterSet.h"
#include "io/IOEnumerations.h"
#include "io/serializer/VolumeSerializer.h"

namespace ettention
{
    class ParameterSource;

    class OutputParameterSet : public ParameterSet
    {
    public:
        OutputParameterSet(VoxelValueType voxelValueType = IO_VOXEL_TYPE_FLOAT_32, CoordinateOrder orientation = ORDER_XYZ, bool invert = false);
        OutputParameterSet(const ParameterSource* parameterSource);
        virtual ~OutputParameterSet();

        const std::filesystem::path& getFilename() const;
        const std::string& getFormat() const;
        VoxelValueType getVoxelType() const;
        CoordinateOrder getOrientation() const;
        bool getInvert() const;
        bool getEnableVoxelization() const;

        void testValidityOfOutputOptions(VolumeSerializer *serializer) const;

    protected:
        void testAvailabilityOfOutputDirectory(std::filesystem::path filename);

    private:
        std::filesystem::path filename;
        std::string format;
        VoxelValueType voxelType;
        CoordinateOrder orientation;
        bool invert;
        bool enableVoxelization;
    };
}