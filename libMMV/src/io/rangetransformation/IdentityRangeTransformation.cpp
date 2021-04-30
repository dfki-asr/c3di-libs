#include "libmmv/io/rangetransformation/IdentityRangeTransformation.h"

namespace libmmv
{
    IdentityRangeTransformation::IdentityRangeTransformation(Volume* volume)
        : RangeTransformation(volume)
    {
    }

    float IdentityRangeTransformation::transformRange(float value)
    {
        return value;
    }

    void IdentityRangeTransformation::transformRange(float* data, size_t size)
    {
    }

};
