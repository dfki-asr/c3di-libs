#include "gtest/gtest.h"
#include "libmmv/algorithm/RandomAlgorithms.h"
#include "libmmv/io/serializer/ImageSerializer.h"
#include "libmmv/model/image/Image.h"
#include "libmmv/model/image/ImageComparator.h"

namespace libmmv
{
    class RandomAlgorithmsTest : public ::testing::Test
    {
    public:
        RandomAlgorithmsTest() {}
        ~RandomAlgorithmsTest() {}

        virtual void generateTestImage(unsigned int width, unsigned int height, unsigned int seed)
        {
            Image* testImage = RandomAlgorithms::generateRandomImage(width, height, seed);
            ImageSerializer::writeImage(std::string(TESTDATA_DIR) + "/actual/random_image/randomTest", testImage, ImageSerializer::TIFF_GRAY_32);
            delete testImage;
        }
    };
}

using namespace libmmv;

TEST_F(RandomAlgorithmsTest, TestRandomImageGeneration)
{
    generateTestImage(256, 256, 42);
    ImageComparator::assertImagesAreEqual(std::string(TESTDATA_DIR) + "/actual/random_image/randomTest.tif", std::string(TESTDATA_DIR) + "/expected/random_image/randomTest.tif");
}