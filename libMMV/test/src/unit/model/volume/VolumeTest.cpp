#include "gtest/gtest.h"
#include "libmmv/model/image/Image.h"
#include "libmmv/model/image/ImageComparator.h"
#include "libmmv/io/deserializer/ImageDeserializer.h"
#include "libmmv/io/serializer/VolumeSerializer.h"
#include "libmmv/model/volume/Volume.h"
#include "libmmv/model/volume/FloatVolume.h"
#include "libmmv/model/volume/HalfFloatVolume.h"
#include "libmmv/model/volume/ByteVolume.h"
#include "libmmv/model/volume/VolumeProperties.h"
#include "libmmv/algorithm/volumestatistics/VolumeStatistics.h"


namespace libmmv
{

    class VolumeTest : public ::testing::Test
    {
    public:

        VolumeTest()
        {
        }

        virtual ~VolumeTest()
        {
        }

        void Test_Voxel_Access(Volume* volume)
        {
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 0, 0)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(15, 15, 15)), 0.0f);

            ASSERT_THROW(volume->getVoxelValue(Vec3ui(16, 0, 0)), std::out_of_range);
            ASSERT_THROW(volume->getVoxelValue(Vec3ui(0, 16, 0)), std::out_of_range);
            ASSERT_THROW(volume->getVoxelValue(Vec3ui(0, 0, 16)), std::out_of_range);

            volume->setVoxelToValue(Vec3ui(0, 0, 0), 1.0f);
            volume->setVoxelToValue(Vec3ui(15, 15, 15), 1.0f);

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 1, 0)), 0.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(15, 14, 15)), 0.0f);

            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(0, 0, 0)), 1.0f);
            ASSERT_FLOAT_EQ(volume->getVoxelValue(Vec3ui(15, 15, 15)), 1.0f);
        }

        void TestComputeMinMaxValues(Volume* volume)
        {
            volume->setVoxelToValue(Vec3ui(0, 0, 0), 1.0f);
            volume->setVoxelToValue(Vec3ui(15, 15, 15), 121.0f);
            volume->setVoxelToValue(Vec3ui(15, 15, 15), 2.0f);

            size_t test = volume->getProperties().getVolumeVoxelCount();

            auto volumeStats = VolumeStatistics::compute(volume);
            ASSERT_FLOAT_EQ(volumeStats.getMin(), 0.0f);
            ASSERT_FLOAT_EQ(volumeStats.getMax(), 2.0f);
            ASSERT_FLOAT_EQ(volumeStats.getMean(), 0.000732421875f);
        }

        void TestSetZSlice(Volume* volume)
        {
            Image* image = ImageDeserializer::readImage(std::string(TESTDATA_DIR) + "/expected/volume/striped_slice.tif");
            volume->setZSlice(2, image);
            delete image;

            for (unsigned int y = 1; y < volume->getProperties().getVolumeResolution().y; y += 2)
            {
                float expectedValueFromImage = (float)((y + 1) / 2);
                ASSERT_FLOAT_EQ(expectedValueFromImage, volume->getVoxelValue(Vec3ui(y, y, 2)));
            }
        }

        void TestSliceExtraction(Volume* volume, unsigned int sliceIndex, CoordinateOrder sliceIsLastOfOrder, Image* referenceSlice)
        {
            auto *volumeSlice = volume->extractSlice(sliceIndex, sliceIsLastOfOrder);
            ImageComparator::assertImagesAreEqual(volumeSlice, referenceSlice);
            delete volumeSlice;
        }

        size_t distanceInBytesBetweenPointers(void* a, void* b)
        {
            return (unsigned char*)b - (unsigned char*)a;
        }

        void writeOutVolume(Volume* volume, std::string filename)
        {
            VolumeSerializer serializer;
            serializer.write(volume, filename, IO_VOXEL_TYPE_FLOAT_32, CoordinateOrder::ORDER_XZY);
        }
    };

    TEST_F(VolumeTest, Test_SliceExtraction_FloatVolume)
    {
        static const float gaussKernel3D[] =
        {
            1.0f, 4.0f, 6.0f, 4.0f, 1.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            6.0f, 24.0f, 36.0f, 24.0f, 6.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            1.0f, 4.0f, 6.0f, 4.0f, 1.0f,

            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            16.0f, 64.0f, 96.0f, 64.0f, 16.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            16.0f, 64.0f, 96.0f, 64.0f, 16.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,

            6.0f, 24.0f, 36.0f, 24.0f, 6.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            36.0f, 144.0f, 216.0f, 144.0f, 36.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            6.0f, 24.0f, 36.0f, 24.0f, 6.0f,

            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            16.0f, 64.0f, 96.0f, 64.0f, 16.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            16.0f, 64.0f, 96.0f, 64.0f, 16.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,

            1.0f, 4.0f, 6.0f, 4.0f, 1.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            6.0f, 24.0f, 36.0f, 24.0f, 6.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            1.0f, 4.0f, 6.0f, 4.0f, 1.0f
        };
        static const float slice1ThroughXZPLane[] =
        {
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f,
            16.0f, 64.0f, 96.0f, 64.0f, 16.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            16.0f, 64.0f, 96.0f, 64.0f, 16.0f,
            4.0f, 16.0f, 24.0f, 16.0f, 4.0f
        };
        static const float slice2ThroughXYPLane[] =
        {
            6.0f, 24.0f, 36.0f, 24.0f, 6.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            36.0f, 144.0f, 216.0f, 144.0f, 36.0f,
            24.0f, 96.0f, 144.0f, 96.0f, 24.0f,
            6.0f, 24.0f, 36.0f, 24.0f, 6.0f
        };

        Volume *volume = new FloatVolume(Vec3ui(5, 5, 5), gaussKernel3D);
        Image *image1 = new Image(Vec2ui(5, 5), slice1ThroughXZPLane);
        Image *image2 = new Image(Vec2ui(5, 5), slice2ThroughXYPLane);

        TestSliceExtraction(volume, 1, ORDER_XZY, image1);      // test general (Volume) slice extraction
        TestSliceExtraction(volume, 2, ORDER_XYZ, image2);      // test special (FloatVolume, XYZ) slice extraction (memcpy)

        
        writeOutVolume(volume, std::string(TESTDATA_DIR) + "/actual/volume/gaussian3dKernel.mrc");

        delete image1;
        delete image2;
        delete volume;
    }

    TEST_F(VolumeTest, Test_Instance_FloatVolume)
    {
        FloatVolume *volume = new FloatVolume(Vec3ui(16, 16, 16), 0.0f);
        Test_Voxel_Access(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_Instance_HalfFloatVolume)
    {
        HalfFloatVolume *volume = new HalfFloatVolume(Vec3ui(16, 16, 16), 0.0f);
        Test_Voxel_Access(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_Instance_ByteVolume)
    {
        ByteVolume *volume = new ByteVolume(Vec3ui(16, 16, 16), 0.0f);
        Test_Voxel_Access(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_Voxel_Access_Float32)
    {
        auto* volume = new FloatVolume(Vec3ui(16, 16, 16), 0.0f);
        Test_Voxel_Access(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_Voxel_Access_HalfFloat16)
    {
        auto* volume = new HalfFloatVolume(Vec3ui(16, 16, 16), 0.0f);
        Test_Voxel_Access(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_Voxel_Access_UChar8)
    {
        auto* volume = new ByteVolume(Vec3ui(16, 16, 16), 0.0f);
        Test_Voxel_Access(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_SetZSlice_Float32)
    {
        auto* volume = new FloatVolume(Vec3ui(16, 16, 16), 0.0f);
        TestSetZSlice(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_SetZSlice_HalfFloat16)
    {
        auto* volume = new HalfFloatVolume(Vec3ui(16, 16, 16), 0.0f);
        TestSetZSlice(volume);
        delete volume;
    }

    TEST_F(VolumeTest, Test_SetZSlice_UChar8)
    {
        auto* volume = new ByteVolume(Vec3ui(16, 16, 16), 0.0f);
        TestSetZSlice(volume);
        delete volume;
    }

}
