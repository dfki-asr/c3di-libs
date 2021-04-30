#include "gtest/gtest.h"
#include "libmmv/algorithm/GeometricAlgorithms.h"
#include "libmmv/model/geometry/Ray.h"
#include "libmmv/math/Vec2.h"
#include "libmmv/math/Vec3.h"
#include "libmmv/math/Vec4.h"

#include <cmath>

namespace libmmv
{
    class GeometricAlgorithmsTest : public ::testing::Test {};
}

using namespace libmmv;

TEST_F(GeometricAlgorithmsTest, ClosestPointOfTwoRays_Case_A)
{
    Vec3f pointA(-1.0f,  0.0f, 0.0f);
    Vec3f dirA(1.0f,  0.0f, 0.0f);

    Vec3f pointB(0.0f, -1.0f, 0.0f);
    Vec3f dirB(0.0f,  1.0f, 0.0f);

    Ray a = Ray::createFromPointAndDirection(pointA, dirA);
    Ray b = Ray::createFromPointAndDirection(pointB, dirB);

    auto result = GeometricAlgorithms::getClosestPointsOfRays(a, b);
    ASSERT_FLOAT_EQ(result.first.x, 0.0f);
    ASSERT_FLOAT_EQ(result.first.y, 0.0f);
    ASSERT_FLOAT_EQ(result.first.z, 0.0f);
    ASSERT_FLOAT_EQ(result.second.x, 0.0f);
    ASSERT_FLOAT_EQ(result.second.y, 0.0f);
    ASSERT_FLOAT_EQ(result.second.z, 0.0f);
}

TEST_F(GeometricAlgorithmsTest, ClosestPointOfTwoRays_Case_B)
{
    Vec3f pointA(-1.0f,  0.0f, 0.0f);
    Vec3f dirA(1.0f,  0.5f, 0.0f);

    Vec3f pointB(0.0f, -1.0f, 0.0f);
    Vec3f dirB(0.5f,  1.0f, 0.0f);

    Ray a = Ray::createFromPointAndDirection(pointA, dirA);
    Ray b = Ray::createFromPointAndDirection(pointB, dirB);

    auto result = GeometricAlgorithms::getClosestPointsOfRays(a, b);
    ASSERT_FLOAT_EQ(result.first.x, 1.0f);
    ASSERT_FLOAT_EQ(result.first.y, 1.0f);
    ASSERT_FLOAT_EQ(result.first.z, 0.0f);
    ASSERT_FLOAT_EQ(result.second.x, 1.0f);
    ASSERT_FLOAT_EQ(result.second.y, 1.0f);
    ASSERT_FLOAT_EQ(result.second.z, 0.0f);
}

TEST_F(GeometricAlgorithmsTest, ClosestPointOfTwoRays_Case_Parallel)
{
    Vec3f pointA(-1.0f, 0.0f, 0.0f);
    Vec3f pointB(0.0f, -1.0f, 0.0f);
    Vec3f dir(1.0f, 0.5f, 0.0f);

    Ray a = Ray::createFromPointAndDirection(pointA, dir);
    Ray b = Ray::createFromPointAndDirection(pointB, dir);

    ASSERT_THROW(GeometricAlgorithms::getClosestPointsOfRays(a, b), std::invalid_argument);
}

TEST_F(GeometricAlgorithmsTest, ClipRayAgainstBoundingBox_Case_Parallel)
{
    Vec3f pointA(0.5f, 0.5f, 0.5f);
    Vec3f dir(1.0f, 0.0f, 0.0f);

    Ray ray = Ray::createFromPointAndDirection(pointA, dir);
    BoundingBox3f aabb(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));

    auto tLimits = GeometricAlgorithms::getClipRayAgainstBoundingBox(ray, aabb);
    ASSERT_FLOAT_EQ(tLimits.first, -0.5f);
    ASSERT_FLOAT_EQ(tLimits.second, 0.5f);
}

TEST_F(GeometricAlgorithmsTest, ClipRayAgainstBoundingBox_Case_Diagonal)
{
    Vec3f pointA(0.5f, 0.5f, 0.5f);
    Vec3f dir(1.0f, 1.0f, 0.0f);

    Ray ray = Ray::createFromPointAndDirection(pointA, dir);
    BoundingBox3f aabb(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));

    auto tLimits = GeometricAlgorithms::getClipRayAgainstBoundingBox(ray, aabb);
    ASSERT_FLOAT_EQ(tLimits.first,  -std::sqrt(2.0f) / 2.0f);
    ASSERT_FLOAT_EQ(tLimits.second,  std::sqrt(2.0f) / 2.0f);
}
