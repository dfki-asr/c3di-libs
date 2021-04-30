#include "libmmv/io/serializer/VolumeFileFormatWriter.h"
#include <fstream>
#include "libmmv/io/rangetransformation/IdentityRangeTransformation.h"
#include "libmmv/io/rangetransformation/LinearRangeTransformation.h"
#include "libmmv/algorithm/volumestatistics/VolumeStatistics.h"
#include "libmmv/model/volume/Volume.h"
#include "libmmv/model/image/Image.h"

namespace libmmv
{

    void VolumeFileFormatWriter::write(Volume* volume, std::string outputVolumeFileName, VoxelValueType voxelMode, CoordinateOrder orientation, bool invert)
    {
        currentVoxelMode = voxelMode;
        currentOrientation = orientation;
        std::ofstream outfile;
        outfile.open(outputVolumeFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        if( !outfile.good() )
        {
            throw std::ios_base::failure("Failed to open output file "+outputVolumeFileName+" for writing!");
        }

        RangeTransformation* rangeTransformation = createRangeTransformation(volume, invert);
        auto volumeDataInFloatFormat = volume->convertToFloat();
        rangeTransformation->transformRange(volumeDataInFloatFormat.get(), volume->getProperties().getVolumeVoxelCount());

        Vec3ui outputVolumeResolution = getOrderDependentResolution(volume->getProperties().getVolumeResolution(), orientation);

        writeHeader(outfile, volume->getProperties(), outputVolumeResolution, rangeTransformation);

        if(currentVoxelMode == IO_VOXEL_TYPE_FLOAT_32)
        {
            writeSlicewise(outfile, outputVolumeResolution, volume);
        } else {
            writePixelwise(outfile, outputVolumeResolution, volume, volumeDataInFloatFormat);
        }

        delete rangeTransformation;

        if( !outfile.good() )
        {
            throw std::ios_base::failure("Error encountered while writing to output file " + outputVolumeFileName);
        }
        outfile.close();
    }

    void VolumeFileFormatWriter::writePixelwise(std::ofstream &outfile, Vec3ui &outputVolumeResolution, Volume* volume, std::unique_ptr<float> &volumeDataInFloatFormat)
    {
        unsigned int orderDependentIndices[3];
        fillOrderDependentIndices(orderDependentIndices, currentOrientation);

        for( unsigned int i = 0; i < outputVolumeResolution.z; ++i )
        {
            for( unsigned int j = 0; j < outputVolumeResolution.y; ++j )
            {
                for( unsigned int k = 0; k < outputVolumeResolution.x; ++k )
                {
                    Vec3ui coords;
                    coords[orderDependentIndices[0]] = i;
                    coords[orderDependentIndices[1]] = j;
                    coords[orderDependentIndices[2]] = k;
                    writeValue(outfile, volumeDataInFloatFormat.get()[volume->getVoxelIndex(coords)]);
                }
            }
        }
    }

    void VolumeFileFormatWriter::writeSlicewise(std::ofstream &output, Vec3ui &outputVolumeResolution, Volume* volume)
    {
        for( unsigned int i = 0; i < outputVolumeResolution.z; ++i )
        {
            auto tempSlice = volume->extractSlice(i, currentOrientation);
            writeSlice(output, tempSlice);
            delete tempSlice;
        }
    }

    void VolumeFileFormatWriter::writeValue(std::ofstream &output, float value)
    {
        short shortValue;
        unsigned short ushortValue;
        unsigned char ucharValue;
        switch(currentVoxelMode)
        {
        case IO_VOXEL_TYPE_FLOAT_32:
            output.write((const char*)&value, sizeof(float));
            break;
        case IO_VOXEL_TYPE_SIGNED_16:
            shortValue = (short)value;
            output.write((const char*)&shortValue, sizeof(short));
            break;
        case IO_VOXEL_TYPE_UNSIGNED_16:
            ushortValue = (unsigned short)value;
            output.write((const char*)&ushortValue, sizeof(unsigned short));
            break;
        case IO_VOXEL_TYPE_UNSIGNED_8:
            ucharValue = (unsigned char)value;
            output.write((const char*)&ucharValue, sizeof(unsigned char));
            break;
        default:
            throw std::runtime_error("Unknown VoxelValueType given!");
        }
    }

    void VolumeFileFormatWriter::writeSlice(std::ofstream &output, Image *image)
    {
        output.write( (const char*)image->getData(), image->getPixelCount() * sizeof(float) );
    }

    RangeTransformation* VolumeFileFormatWriter::createRangeTransformation(Volume* volume, bool invert)
    {
        VolumeStatistics statistics;
        if( invert )
        {
            switch(currentVoxelMode)
            {
            case IO_VOXEL_TYPE_UNSIGNED_8:
                return new LinearRangeTransformation(volume, 255.0f, 0.0f);
            case IO_VOXEL_TYPE_UNSIGNED_16:
                return new LinearRangeTransformation(volume, 65535.0f, 0.0f);
            case IO_VOXEL_TYPE_SIGNED_16:
                return new LinearRangeTransformation(volume, 32767.0f, -32768.0f);
            case IO_VOXEL_TYPE_FLOAT_32:
                statistics = VolumeStatistics::compute(volume);
                return new LinearRangeTransformation(volume, statistics.getMax(), statistics.getMin());
            default:
                throw std::runtime_error("range transformation for unknown file mode requested");
                break;
            }
        } else
        {
            switch(currentVoxelMode)
            {
            case IO_VOXEL_TYPE_UNSIGNED_8:
                return new LinearRangeTransformation(volume, 0.0f, 255.0f);
            case IO_VOXEL_TYPE_UNSIGNED_16:
                return new LinearRangeTransformation(volume, 0.0f, 65535.0f);
            case IO_VOXEL_TYPE_SIGNED_16:
                return new LinearRangeTransformation(volume, -32768.0f, 32767.0f);
            case IO_VOXEL_TYPE_FLOAT_32:
                return new IdentityRangeTransformation(volume);
            default:
                throw std::runtime_error("range transformation for unknown file mode requested");
                break;
            }
        }
    }

}