#include "stdafx.h"

#include "gtest/gtest.h"
#include "algorithm/RandomAlgorithms.h"
#include "io/deserializer/ImageDeserializer.h"
#include "io/serializer/ImageSerializer.h"
#include "model/image/Image.h"
#include "model/image/ImageComparator.h"
#include "model/volume/Voxel.h"

#include "evaluation/RootMeanSquareError.h"
#include "evaluation/SignalToNoiseRatio.h"
#include "evaluation/StatisticalStandardMeasures.h"
#include "evaluation/StructuralCorrelation.h"
#include "evaluation/StructuralSimilarityIndex.h"

namespace ettention
{
    class EvaluationTest : public ::testing::Test
    {
    public:
        EvaluationTest() {}
        ~EvaluationTest() {}
    };
}

using namespace ettention;

TEST_F(EvaluationTest, MeanSquareErrorTest)
{
    std::vector<float> v1{ 1, 7, 9, -4, -8, 4, 0, -2, 2, -17 };
    std::vector<float> v2{ 3, 2, -3, -5, 2, 0, 2, -1, 1, -9 };

    float msError = RootMeanSquareError::computeMSError(v1, v2);
    ASSERT_FLOAT_EQ(msError, 36.0f);
}

TEST_F(EvaluationTest, RootMeanSquareErrorTest)
{
    std::vector<float> v1{ 1, 7, 9, -4, -8, 4, 0, -2, 2, -17 };
    std::vector<float> v2{ 3, 2, -3, -5, 2, 0, 2, -1, 1, -9 };

    float msError = RootMeanSquareError::computeRMSError(v1, v2);
    ASSERT_FLOAT_EQ(msError, 6.0f);
}

TEST_F(EvaluationTest, MeanTest)
{
    std::vector<float> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    float meanValue = StatisticalStandardMeasures::computeMean(v);
    ASSERT_FLOAT_EQ(meanValue, 5.0f);
}

TEST_F(EvaluationTest, VarianceWithMaskTest)
{
    std::vector<float> v{
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        1, 2, 3, 4, 5, 6, 7, 8, 9
    };
    std::vector<byte_t> m{
          0,   0,   0,   0,   0,   0,   0,   0,   0,
        255, 255, 255, 255, 255, 255, 255, 255, 255,
          0,   0,   0,   0,   0,   0,   0,   0,   0
    };

    float variance = StatisticalStandardMeasures::computeVarianceUsingMask(&v[0], &m[0], 255, v.size());
    ASSERT_FLOAT_EQ(variance, 7.5f);
}

TEST_F(EvaluationTest, VarianceTest)
{
    std::vector<float> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    float varianceValue = StatisticalStandardMeasures::computeVariance(v, false);
    ASSERT_FLOAT_EQ(varianceValue, 6.666666f);

    varianceValue = StatisticalStandardMeasures::computeVariance(v);
    ASSERT_FLOAT_EQ(varianceValue, 7.5f);
}

TEST_F(EvaluationTest, StandardDeviationTest)
{
    std::vector<float> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    float stDeviation = StatisticalStandardMeasures::computeStandardDeviation(v, false);
    ASSERT_FLOAT_EQ(stDeviation, 2.5819888f);

    stDeviation = StatisticalStandardMeasures::computeStandardDeviation(v);
    ASSERT_FLOAT_EQ(stDeviation, 2.7386129f);
}

TEST_F(EvaluationTest, CovarianceTest)
{
    std::vector<float> v1{ 1, 7, 9, -4, -8, 4, 0, -2, 2, -17 };
    std::vector<float> v2{ 3, 2, -3, -5, 2, 0, 2, -1, 1, -9 };

    float coVariance = StatisticalStandardMeasures::computeCovariance(v1, v2, false);
    ASSERT_FLOAT_EQ(coVariance, 14.459998f);

    coVariance = StatisticalStandardMeasures::computeCovariance(v1, v2);
    ASSERT_FLOAT_EQ(coVariance, 16.06666f);
}

TEST_F(EvaluationTest, SignalToNoiseRatioTest)
{
    std::vector<float> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    float snr = SignalToNoiseRatio::computeSignalToNoiseRatio(v);
    ASSERT_FLOAT_EQ(snr, 1.8257418f);
}

TEST_F(EvaluationTest, PeakSignalToNoiseRatioTest)
{
    std::vector<float> v1{ 1, 7, 9, -4, -8, 4, 0, -2, 2, -17 };
    std::vector<float> v2{ 3, 2, -3, -5, 2, 0, 2, -1, 1, -9 };

    float psnr = SignalToNoiseRatio::computePeakSignalToNoiseRatio(v1, v2, 8.0f);
    ASSERT_FLOAT_EQ(psnr, 32.567776f);
}


TEST_F(EvaluationTest, ImprovedSignalToNoiseRatioTest)
{
    std::vector<float> v1{ 1, 7, 9, -4, -8, 4, 0, -2, 2, -17 };
    std::vector<float> v2{ 3, 2, -3, -5, 2, 0, 2, -1, 1, -9 };

    float isnr = SignalToNoiseRatio::computeImprovedSignalToNoiseRatio(v1, v2);
    ASSERT_FLOAT_EQ(isnr, -4.1642342f);
}

TEST_F(EvaluationTest, StructuralSimilarityIndexTest)
{
    std::vector<float> v1{ 1, 7, 9, -4, -8, 4, 0, -2, 2, -17 };
    std::vector<float> v2{ 3, 2, -3, -5, 2, 0, 2, -1, 1, -9 };

    float ssim = StructuralSimilarityIndex::computeStructuralSimilarityIndex(v1, v2, 8);
    ASSERT_FLOAT_EQ(ssim, 3174.0469f);

    Image* input1 = ImageDeserializer::readImage(std::string(TESTDATA_DIR) + "/expected/evaluation/SSIMtestInput1.tif");
    Image* input2 = ImageDeserializer::readImage(std::string(TESTDATA_DIR) + "/expected/evaluation/SSIMtestInput2.tif");

    ssim = StructuralSimilarityIndex::computeStructuralSimilarityIndex(input1->getData(), input2->getData(), (size_t)input1->getPixelCount(), 8);
    ASSERT_NEAR(ssim, 64.075005f, 0.01f);

    delete input1;
    delete input2;
}

TEST_F(EvaluationTest, StructuralCorrelationTest)
{
    std::vector<float> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::vector<float> v2{ 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    float sc = StructuralCorrelation::computeStructuralCorrelation(v1, v2);
    ASSERT_FLOAT_EQ(sc, 0.81818181f);

    Image* input1 = ImageDeserializer::readImage(std::string(TESTDATA_DIR) + "/expected/evaluation/SSIMtestInput1.tif");
    Image* input2 = ImageDeserializer::readImage(std::string(TESTDATA_DIR) + "/expected/evaluation/SSIMtestInput2.tif");

    sc = StructuralCorrelation::computeStructuralCorrelation(input1->getData(), input2->getData(), (size_t)input1->getPixelCount());
    ASSERT_NEAR(sc, 0.95252919f, 0.0001f);

    delete input1;
    delete input2;
}