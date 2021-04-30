#include "libmmv/io/serializer/VolumeSerializer.h"
#include "libmmv/io/serializer/MRCWriter.h"

namespace libmmv
{
    VolumeSerializer::VolumeSerializer()
    {
        registerVolumeFileFormat(new MRCWriter());
    }

    VolumeSerializer::~VolumeSerializer()
    {
        for(auto it = fileFormats.begin(); it != fileFormats.end(); ++it)
            delete it->second;
    }

    void VolumeSerializer::registerVolumeFileFormat(VolumeFileFormatWriter* writer)
    {
        fileFormats[writer->getFileExtension()] = writer;
    }

    VolumeFileFormatWriter* VolumeSerializer::findWriter(std::string specifiedExtension, std::string outputVolumeFileName)
    {
        auto formatEntry = fileFormats.find(specifiedExtension);
        if(formatEntry != fileFormats.end())
            return formatEntry->second;

        size_t idx = outputVolumeFileName.rfind('.');
        if (idx != std::string::npos) {
            specifiedExtension = outputVolumeFileName.substr(idx+1);
        }
        else {
            specifiedExtension = "No extension found in filename";
        }

        formatEntry = fileFormats.find(specifiedExtension);
        if(formatEntry == fileFormats.end())
            throw std::ios_base::failure("No suitable serializer for volume file format " + specifiedExtension + ". Plugin missing?");

        return formatEntry->second;
    }

    void VolumeSerializer::write(Volume* volume, std::string outputVolumeFileName, VoxelValueType mode, CoordinateOrder order)
    {
        VolumeFileFormatWriter* writer = findWriter(outputVolumeFileName.substr(outputVolumeFileName.find_last_of('.')), outputVolumeFileName);
        writer->write(volume, outputVolumeFileName, mode, order);
    }

    void VolumeSerializer::write(Volume* volume, std::string outputVolumeFileName, std::string specificFileExtension, VoxelValueType mode, CoordinateOrder order) {
        VolumeFileFormatWriter* writer = findWriter(specificFileExtension, outputVolumeFileName);
        writer->write(volume, outputVolumeFileName, mode, order);
    }
}