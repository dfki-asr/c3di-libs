#include "libCFG/setup/parameterset/OutputParameterSet.h"
#include "libCFG/setup/parametersource/ParameterSource.h"
#include "libmmv/model/volume/FloatVolume.h"
#include <fstream>
#include <cstdio>

namespace libCFG
{
    OutputParameterSet::OutputParameterSet(libmmv::VoxelValueType voxelValueType, libmmv::CoordinateOrder orientation, bool invert)
        : voxelType(voxelValueType)
        , orientation(orientation)
        , invert(invert)
    {
    }

    OutputParameterSet::OutputParameterSet(const ParameterSource* parameterSource)
    {
        filename = parameterSource->getPathParameter("output.filename");
        testAvailabilityOfOutputDirectory(filename);

        format = "";
        if(parameterSource->parameterExists("output.format"))
        {
            format = "." + parameterSource->getStringParameter("output.format");
        }

        voxelType = libmmv::IO_VOXEL_TYPE_FLOAT_32;
        if(parameterSource->parameterExists("output.options.voxelType"))
        {
            voxelType = libmmv::parseVoxelValueType(parameterSource->getStringParameter("output.options.voxelType"));
        }

        orientation = libmmv::ORDER_XZY;
        if(parameterSource->parameterExists("output.options.orientation"))
        {
            orientation = libmmv::parseVoxelCoordinateOrder(parameterSource->getStringParameter("output.options.orientation"));
        }

        invert = false;
        if(parameterSource->parameterExists("output.options.invert"))
        {
            invert = parameterSource->getBoolParameter("output.options.invert");
        }

        enableVoxelization = false;
        if(parameterSource->parameterExists("output.options.enableVoxelization"))
        {
            enableVoxelization = parameterSource->getBoolParameter("output.options.enableVoxelization");
        }
    }

    OutputParameterSet::~OutputParameterSet()
    {
    }

    const std::filesystem::path& OutputParameterSet::getFilename() const
    {
        return filename;
    }

    const std::string& OutputParameterSet::getFormat() const
    {
        return format;
    }

    libmmv::VoxelValueType OutputParameterSet::getVoxelType() const
    {
        return voxelType;
    }

    libmmv::CoordinateOrder OutputParameterSet::getOrientation() const
    {
        return orientation;
    }

    bool OutputParameterSet::getInvert() const
    {
        return invert;
    }

    bool OutputParameterSet::getEnableVoxelization() const
    {
        return enableVoxelization;
    }

    void OutputParameterSet::testAvailabilityOfOutputDirectory(std::filesystem::path filename)
    {
        std::ofstream outf;
        try
        {
            outf.exceptions(std::ifstream::failbit | std::ofstream::badbit);
            outf.open(filename.string(), std::ofstream::trunc);
            outf << "Test output folder availability.";
            outf.close();
            remove(filename.string().c_str());
        }
        catch (...)
        {
            throw std::ios_base::failure( "Cannot write to output path. Check its availability and/or permissions. Path: " + filename.string() );
        }
    }

    void OutputParameterSet::testValidityOfOutputOptions(libmmv::VolumeSerializer *serializer) const
    {
        auto *volume = new libmmv::FloatVolume(libmmv::Vec3ui(8, 4, 2), 0.0f);
        serializer->write(volume, filename.string(), format, voxelType, orientation);
        delete volume;

        remove(filename.string().c_str());
    }

}