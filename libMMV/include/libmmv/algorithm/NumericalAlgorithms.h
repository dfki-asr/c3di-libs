#pragma once
#include "libmmv/math/Vec2.h"
#include "libmmv/math/Vec3.h"

namespace libmmv
{
    class NumericalAlgorithms
    {
    public:
        static float degreeToRadians(float degree);
        static float radiansToDegree(float radians);
        static unsigned int nearestPowerOfTwo(unsigned int number);
        static Vec2ui nearestPowerOfTwo(Vec2ui resolution);
        static unsigned long long factorial(unsigned int n);
        static double uintPow(double base, unsigned int exponent);
        static float getIntersectionLengthOfRayThroughVoxelCenter(const Vec3f& rayDir, const Vec3f& voxelSize);
        static std::string bytesToString(std::size_t bytes);
        static void assertValueIsFinite(float value);
    };
}