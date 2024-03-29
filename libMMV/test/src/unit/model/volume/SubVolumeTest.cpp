#include "gtest/gtest.h"
#include "libmmv/io/serializer/VolumeSerializer.h"
#include "libmmv/io/deserializer/ImageDeserializer.h"
#include "libmmv/model/volume/Volume.h"
#include "libmmv/model/volume/SubVolume.h"
#include "libmmv/model/volume/FloatVolume.h"
#include "libmmv/model/volume/HalfFloatVolume.h"
#include "libmmv/model/volume/VolumeProperties.h"
#include "libmmv/model/volume/Voxel.h"

namespace libmmv
{
    class SubVolumeTest : public ::testing::Test
    {
    public:
        SubVolumeTest()
        {
        }

        virtual ~SubVolumeTest()
        {
        }

        void testSequentialSubvolumeAccess(SubVolume* subVolume, size_t expectedSizeOfSubvolumeInBytes)
        {
            subVolume->setCurrentSubVolume(0);
            unsigned char* subVolume0Start = (unsigned char*)subVolume->getData();
            subVolume->setCurrentSubVolume(7);
            unsigned char* subVolume7Start = (unsigned char*)subVolume->getData();

            ASSERT_EQ(distanceInBytesBetweenPointers(subVolume0Start, subVolume7Start), expectedSizeOfSubvolumeInBytes * 7);

            ASSERT_THROW(subVolume->setCurrentSubVolume(-1); , std::out_of_range);
            ASSERT_THROW(subVolume->setCurrentSubVolume(8); , std::out_of_range);
        }

        void testArbitrarySubvolume(SubVolume* subVolume)
        {
            subVolume->setArbitrarySubVolume(Vec3i(-1, 0, 0), Vec3ui(2, 2, 2));
            auto result = subVolume->convertToFloat();
            float* resultPtr = result.get();
            ASSERT_FLOAT_EQ(resultPtr[0], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[1], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[2], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[3], 16.0f);
            ASSERT_FLOAT_EQ(resultPtr[4], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[5], 256.0f);
            ASSERT_FLOAT_EQ(resultPtr[6], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[7], 272.0f);
            result = NULL;

            subVolume->setArbitrarySubVolume(Vec3i(0, -1, 0), Vec3ui(2, 2, 2));
            result = subVolume->convertToFloat();
            resultPtr = result.get();
            ASSERT_FLOAT_EQ(resultPtr[0], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[1], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[2], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[3], 1.0f);
            ASSERT_FLOAT_EQ(resultPtr[4], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[5], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[6], 256.0f);
            ASSERT_FLOAT_EQ(resultPtr[7], 257.0f);
            result = NULL;

            subVolume->setArbitrarySubVolume(Vec3i(0, 0, -1), Vec3ui(2, 2, 2));
            result = subVolume->convertToFloat();
            resultPtr = result.get();
            ASSERT_FLOAT_EQ(resultPtr[0], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[1], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[2], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[3], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[4], 0.0f);
            ASSERT_FLOAT_EQ(resultPtr[5], 1.0f);
            ASSERT_FLOAT_EQ(resultPtr[6], 16.0f);
            ASSERT_FLOAT_EQ(resultPtr[7], 17.0f);
        }

        void testArbitrarySubvolumeCopyBack(Volume* volume, SubVolume* subVolume)
        {
            subVolume->setArbitrarySubVolume(Vec3i(1, 1, 1), Vec3ui(2, 2, 2));

            subVolume->fillMemory(0.0f);
            subVolume->copyPrivateDataToVolume();

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 0, 0)), 0.0f);

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(1, 0, 0)), 1.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 1, 0)), 16.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 0, 1)), 256.0f);

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(1, 1, 1)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(1, 1, 2)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(1, 2, 1)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(1, 2, 2)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(2, 1, 1)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(2, 1, 2)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(2, 2, 1)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(2, 2, 2)), 0.0f);
        }

        void testArbitrarySubvolumeCopyBackBorderCase(Volume* volume, SubVolume* subVolume)
        {
            subVolume->setArbitrarySubVolume(Vec3i(15, 15, 15), Vec3ui(2, 2, 2));

            subVolume->fillMemory(0.0f);
            subVolume->copyPrivateDataToVolume();

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 0, 0)), 0.0f);

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(1, 0, 0)), 1.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 1, 0)), 16.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 0, 1)), 256.0f);

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(15, 15, 15)), 0.0f);
        }

        void initVolumeToIndex(Volume* volume)
        {
            Vec3ui pos;
            for(pos.z = 0; pos.z < volume->getProperties().getVolumeResolution().z; pos.z++)
            {
                for(pos.y = 0; pos.y < volume->getProperties().getVolumeResolution().y; pos.y++)
                {

                    for(pos.x = 0; pos.x < volume->getProperties().getVolumeResolution().x; pos.x++)
                    {
                        float value = (float)volume->getVoxelIndex(pos);
                        volume->setVoxelToValue(pos, value);
                    }
                }
            }
        }

        size_t distanceInBytesBetweenPointers(void* a, void* b) 
        {
            return (unsigned char*)b - (unsigned char*)a;
        }
    };
}

using namespace libmmv;

TEST_F(SubVolumeTest, Test_SequentialSubvolumeAccess_Float32)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new FloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);

    SubVolume* subVolume = new SubVolume(volume);
    testSequentialSubvolumeAccess(subVolume, 16 * 16 * 16 / 8 * sizeof(float));

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_SequentialSubvolumeAccess_HalfFloat16)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new HalfFloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);

    SubVolume* subVolume = new SubVolume(volume);

    testSequentialSubvolumeAccess(subVolume, 16 * 16 * 16 / 8 * sizeof(unsigned short));

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_ArbitrarySubvolumeAccess_Float32)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new FloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);
    initVolumeToIndex(volume);

    SubVolume* subVolume = new SubVolume(volume);

    subVolume->setArbitrarySubVolume(Vec3i(0, 0, 0), Vec3ui(2, 2, 2));
    auto result = subVolume->convertToFloat();
    float* resultPtr = result.get();
    ASSERT_FLOAT_EQ(resultPtr[0], 0.0f);
    ASSERT_FLOAT_EQ(resultPtr[1], 1.0f);
    ASSERT_FLOAT_EQ(resultPtr[2], 16.0f);
    ASSERT_FLOAT_EQ(resultPtr[3], 17.0f);
    ASSERT_FLOAT_EQ(resultPtr[4], 256.0f);
    ASSERT_FLOAT_EQ(resultPtr[5], 257.0f);
    ASSERT_FLOAT_EQ(resultPtr[6], 272.0f);
    ASSERT_FLOAT_EQ(resultPtr[7], 273.0f);

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_ArbitrarySubvolumeBorderCases_Float32)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new FloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);
    initVolumeToIndex(volume);

    SubVolume* subVolume = new SubVolume(volume);

    testArbitrarySubvolume(subVolume);

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_ArbitrarySubvolumeBorderCases_HalfFloat16)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new HalfFloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);
    initVolumeToIndex(volume);

    SubVolume* subVolume = new SubVolume(volume);

    testArbitrarySubvolume(subVolume);

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_ArbitrarySubvolumeCopyBack_Float32)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new FloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);
    initVolumeToIndex(volume);

    SubVolume* subVolume = new SubVolume(volume);

    testArbitrarySubvolumeCopyBack(volume, subVolume);

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_ArbitrarySubvolumeCopyBack_HalfFloat16)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new HalfFloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);
    initVolumeToIndex(volume);

    SubVolume* subVolume = new SubVolume(volume);

    testArbitrarySubvolumeCopyBack(volume, subVolume);

    delete subVolume;
    delete volume;
}

TEST_F(SubVolumeTest, Test_ArbitrarySubvolumeCopyBack_Bordercases_Float32)
{
    Vec3ui resolution(16, 16, 16);
    auto* volume = new FloatVolume(resolution, 0.0f);
    volume->setSubVolumeCount(8);
    initVolumeToIndex(volume);

    SubVolume* subVolume = new SubVolume(volume);

    testArbitrarySubvolumeCopyBack(volume, subVolume);

    delete subVolume;
    delete volume;
}