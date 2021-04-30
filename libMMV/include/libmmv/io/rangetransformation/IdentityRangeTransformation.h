#pragma once
#include <cstddef>
#include "libmmv/io/rangetransformation/RangeTransformation.h"

namespace libmmv
{
    class Volume;

    class IdentityRangeTransformation : public RangeTransformation
    {
    public:
        IdentityRangeTransformation(Volume* volume);

        virtual float transformRange(float value) override;
        virtual void transformRange(float* data, std::size_t size) override;
    };
};
