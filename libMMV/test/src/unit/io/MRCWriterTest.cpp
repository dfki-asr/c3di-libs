#include "gtest/gtest.h"
#include <filesystem>
#include "libmmv/io/serializer/VolumeSerializer.h"
#include "libmmv/model/volume/Voxel.h"
#include "libmmv/model/volume/Volume.h"
#include "libmmv/model/volume/FloatVolume.h"
#include "libmmv/io/datasource/MRCHeader.h"
#include "libmmv/io/serializer/MRCWriter.h"
#include "libmmv/algorithm/volumestatistics/VolumeStatistics.h"

using namespace libmmv;

class MRCWriterTest : public ::testing::Test
{
public:
    Volume* volume;

    MRCWriterTest()
        : volume(0)
    {
    }

    ~MRCWriterTest()
    {
    }

    void TearDown() override
    {
        delete volume;
        ::testing::Test::TearDown();
    }

    void allocateVolume(Voxel::DataType voxelType)
    {
        volume = new FloatVolume(Vec3ui(16, 16, 16), 0.0f);
        volume->setVoxelToValue(Vec3ui(0, 0, 0), 1.0f);
        volume->setVoxelToValue(Vec3ui(15, 0, 0), 2.0f);
        volume->setVoxelToValue(Vec3ui(0, 15, 0), 3.0f);
        volume->setVoxelToValue(Vec3ui(0, 0, 15), 4.0f);
    }
};

TEST_F(MRCWriterTest, Test_Float32)
{
    this->allocateVolume(Voxel::DataType::FLOAT_32);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/float_32.mrc" , IO_VOXEL_TYPE_FLOAT_32, ORDER_XYZ);
}

TEST_F(MRCWriterTest, Test_Unsigned_16)
{
    this->allocateVolume(Voxel::DataType::FLOAT_32);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/uint_16.mrc", IO_VOXEL_TYPE_UNSIGNED_16, ORDER_XYZ);
}

TEST_F(MRCWriterTest, Test_Signed_16)
{
    this->allocateVolume(Voxel::DataType::FLOAT_32);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/int_16.mrc", IO_VOXEL_TYPE_SIGNED_16, ORDER_XYZ);
}

TEST_F(MRCWriterTest, Test_Unsigned_8)
{
    this->allocateVolume(Voxel::DataType::FLOAT_32);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/uint_8.mrc", IO_VOXEL_TYPE_UNSIGNED_8, ORDER_XYZ);
}

TEST_F(MRCWriterTest, Test_Memory_Float16_Disk_Float32)
{
    this->allocateVolume(Voxel::DataType::HALF_FLOAT_16);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/float_32_internal_halffloat_16.mrc", IO_VOXEL_TYPE_FLOAT_32, ORDER_XYZ);
}

TEST_F(MRCWriterTest, Test_Unsigned_16_Inverted)
{
    this->allocateVolume(Voxel::DataType::HALF_FLOAT_16);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/uint16_inverted.mrc", IO_VOXEL_TYPE_UNSIGNED_16, ORDER_XYZ, true);
}

TEST_F(MRCWriterTest, Test_Float_32_Inverted)
{
    this->allocateVolume(Voxel::DataType::HALF_FLOAT_16);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/float_32_inverted.mrc", IO_VOXEL_TYPE_FLOAT_32, ORDER_XYZ, true);
}

TEST_F(MRCWriterTest, Test_Float32_XZY)
{
    this->allocateVolume(Voxel::DataType::HALF_FLOAT_16);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/uint16_order_xzy.mrc", IO_VOXEL_TYPE_FLOAT_32, ORDER_XZY);
}

TEST_F(MRCWriterTest, Test_UInt16_XZY)
{
    this->allocateVolume(Voxel::DataType::HALF_FLOAT_16);
    MRCWriter writer;
    writer.write(volume, std::string(TESTDATA_DIR) + "/actual/volume/float32_order_xzy.mrc", IO_VOXEL_TYPE_UNSIGNED_16, ORDER_XZY);
}