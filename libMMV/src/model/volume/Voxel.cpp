#include "stdafx.h"

#include "model/volume/Voxel.h"

namespace ettention
{
    size_t Voxel::sizeOfVoxelType(const DataType type)
    {
        switch( type )
        {
        case DataType::UCHAR_8:         return sizeof(byte_t);
        case DataType::HALF_FLOAT_16:  return sizeof(half_float_t);
        case DataType::FLOAT_32:       return sizeof(float);
        default:
            throw std::invalid_argument("invalid voxel type");
        }
    }
	
    std::string Voxel::makeKernelDefine(const DataType type)
    {
        switch( type )
        {
        case DataType::UCHAR_8:             return " -D UCHAR_8";
        case DataType::HALF_FLOAT_16:       return " -D HALF_FLOAT_16";
        case DataType::FLOAT_32:            return " -D FLOAT_32";
        default:
            throw std::invalid_argument("invalid voxel type");
        }
    }

    Voxel::DataType Voxel::parseVoxelType(const std::string value)
    {
        if( value == "uchar" )
        {
            return DataType::UCHAR_8;
        }
        if( value == "half" )
        {
            return DataType::HALF_FLOAT_16;
        }
        if( value == "float" )
        {
            return DataType::FLOAT_32;
        }
        throw std::invalid_argument("Illegal parameter value for voxel representation: " + value);
    }
}
