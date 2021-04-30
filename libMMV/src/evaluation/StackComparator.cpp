#include "libmmv/evaluation/StackComparator.h"
#include "libmmv/model/volume/Volume.h"
#include "libmmv/io/datasource/MRCDataSource.h"
#include "libmmv/evaluation/RootMeanSquareError.h"

namespace libmmv
{
    const float StackComparator::acceptableRMSError = 0.011f;

    StackComparator::StackComparator(ImageStackDataSource& firstInputStack, ImageStackDataSource& secondInputStack) :
        firstStack(firstInputStack),
        secondStack(secondInputStack)
    {
    }

    StackComparator::~StackComparator()
    {
    }

    float StackComparator::getRMSOfSlice(unsigned int sliceIndex)
    {
        auto sliceDataFromFirstStack = firstStack.loadProjectionImage(sliceIndex);
        auto sliceDataFromSecondStack = secondStack.loadProjectionImage(sliceIndex);
        float rmsError = RootMeanSquareError::computeRMSError(sliceDataFromFirstStack->getData(), sliceDataFromSecondStack->getData(), sliceDataFromFirstStack->getPixelCount());
        return rmsError;
    }

    unsigned int StackComparator::getSliceIndexWithMaxRMS()
    {
        float rmsError = 0.f;
        float maxRMSError = -1.f;
        unsigned int sliceIndexWithMaxRMS = 0;

        for(unsigned int i = 0; i<firstStack.getNumberOfProjections(); i++)
        {
            auto sliceDataFromFirstStack = firstStack.loadProjectionImage(i);
            auto sliceDataFromSecondStack = secondStack.loadProjectionImage(i);
            rmsError = RootMeanSquareError::computeRMSError(sliceDataFromFirstStack->getData(), sliceDataFromSecondStack->getData(), sliceDataFromFirstStack->getPixelCount());

            if(rmsError>maxRMSError)
            {
                maxRMSError = rmsError;
                sliceIndexWithMaxRMS = i;
            }
        }

        return sliceIndexWithMaxRMS;
    }

    float StackComparator::getMaxRMS()
    {
        float rmsError = 0.f;
        float maxRMSError = -1.f;

        for(unsigned int i = 0; i < firstStack.getNumberOfProjections(); i++)
        {
            auto sliceDataFromFirstStack = firstStack.loadProjectionImage(i);
            auto sliceDataFromSecondStack = secondStack.loadProjectionImage(i);
            rmsError = RootMeanSquareError::computeRMSError(sliceDataFromFirstStack->getData(), sliceDataFromSecondStack->getData(), sliceDataFromFirstStack->getPixelCount());
            if(rmsError>maxRMSError)
            {
                maxRMSError = rmsError;
            }
        }
        return maxRMSError;
    }

    float StackComparator::getRMSBetweenVolumes(std::string firstVolume, std::string secondVolume)
    {
        MRCDataSource firstStack(firstVolume, false);
        MRCDataSource secondStack(secondVolume, false);
        if(firstStack.getResolution() != secondStack.getResolution() ||
           firstStack.getNumberOfProjections() != secondStack.getNumberOfProjections())
        {
            throw std::runtime_error("illegal comparison of volumes with different size");
        }
        StackComparator comparator(firstStack, secondStack);
        const float rms = comparator.getMaxRMS();
        return rms;
    }

    void StackComparator::assertVolumesAreEqual(std::string firstVolume, std::string secondVolume)
    {
        const float rms = getRMSBetweenVolumes(firstVolume, secondVolume);
        if(rms > acceptableRMSError)
        {
            std::stringstream err;
            err << "volumes\n\t" << firstVolume << " and \n\t" << secondVolume << "difference: " << rms;
            throw std::runtime_error(err.str());
        }
    }

    float StackComparator::getRMSBetweenVolumes(Volume* firstVolume, Volume* secondVolume)
    {
        if(firstVolume->getProperties().getVolumeResolution() != secondVolume->getProperties().getVolumeResolution())
            throw std::runtime_error("illegal comparison of volumes with different size");

        auto dataOne = firstVolume->convertToFloat();
        auto dataTwo = secondVolume->convertToFloat();

        return RootMeanSquareError::computeRMSError(dataOne.get(), dataTwo.get(), firstVolume->getProperties().getVolumeVoxelCount());
    }

    void StackComparator::assertVolumesAreEqual(Volume* firstVolume, Volume* secondVolume)
    {
        const float rms = getRMSBetweenVolumes(firstVolume, secondVolume);
        if(rms > acceptableRMSError)
        {
            std::stringstream err;
            err << "volumes differ by " << rms;
            throw std::runtime_error(err.str());
        }
    }
}