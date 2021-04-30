#include "libmmv/evaluation/DistanceBetweenConsecutiveSlabs.h"
#include "libmmv/evaluation/RootMeanSquareError.h"
#include "libmmv/model/image/Image.h"
#include "libmmv/model/volume/FloatVolume.h"

namespace libmmv
{

    float DistanceBetweenConsecutiveSlabs::l2NormDistance( Volume* volume, unsigned int slabSize )
    {
        unsigned int numberOfSlices = volume->getProperties().getVolumeResolution().z;
        unsigned int pixelPerSlice = volume->getProperties().getVolumeResolution().x * volume->getProperties().getVolumeResolution().y;
        unsigned int numSlabs = numberOfSlices / slabSize;
        if (numSlabs * slabSize != numberOfSlices)
            throw std::runtime_error("volume z-resolution is not a multiple of slabsize");

        float error = 0.0f;
        float count = 0.0f;
        for (unsigned int slabNumber = 0; slabNumber < numSlabs-1; slabNumber++)
        {
            for (unsigned int sliceInsideSlab = 0; sliceInsideSlab < slabSize; sliceInsideSlab++)
            {
                auto imageA = volume->extractSlice( slabNumber    * slabSize + sliceInsideSlab);
                auto imageB = volume->extractSlice((slabNumber+1) * slabSize + sliceInsideSlab);

                error += RootMeanSquareError::computeRMSError( imageA->getData(), imageB->getData(), pixelPerSlice );
                count += 1.0f;

                delete imageA;
                delete imageB;
            }
        }
        return error / count;
    }
}