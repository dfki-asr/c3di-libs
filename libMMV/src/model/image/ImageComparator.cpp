#include "stdafx.h"

#include "ImageComparator.h"

#include "evaluation/RootMeanSquareError.h"
#include "io/deserializer/ImageDeserializer.h"

namespace ettention
{
    ImageComparator::ImageComparator()
    {
    }

    ImageComparator::~ImageComparator()
    {
    }

    bool ImageComparator::areHeadersEqual(Image* first, Image* second)
    {
        return first->getResolution() == second->getResolution();
    }

    float ImageComparator::getRMS(std::string firstFilename, std::string secondFilename)
    {
        auto firstImage = std::unique_ptr<Image>(ImageDeserializer::readImage(firstFilename));
        auto secondImage = std::unique_ptr<Image>(ImageDeserializer::readImage(secondFilename));
        float result = getRMS(firstImage.get(), secondImage.get());
        return result;
    }

    float ImageComparator::getRMS(Image* first, Image* second)
    {
        if(first->getResolution() != second->getResolution())
            throw std::runtime_error("Image dimensions do not match!");

        return RootMeanSquareError::computeRMSError(first->getData(), second->getData(), first->getPixelCount());
    }

    float ImageComparator::getMaxError(std::string firstFilename, std::string secondFilename)
    {
        auto first = std::unique_ptr<Image>(ImageDeserializer::readImage(firstFilename));
        auto second = std::unique_ptr<Image>(ImageDeserializer::readImage(secondFilename));

        float result = ImageComparator::getMaxError(first.get(), second.get());
        return result;
    }

    float ImageComparator::getMaxError(Image* first, Image* second)
    {
        float result = 0.0f;
        float rmsError = 0.0f;

        if(first->getResolution() != second->getResolution())
            throw std::runtime_error("Illegal comparison of images with different resolution!");

        float minValue = FLT_MAX;
        float maxValue = -FLT_MAX;

        for(unsigned int i = 0; i < first->getPixelCount(); i++)
        {
            if(!std::isfinite(first->getData()[i]))
                throw std::runtime_error("illegal value in data");
            if(!std::isfinite(second->getData()[i]))
                throw std::runtime_error("illegal value in data");

            if(second->getData()[i] < minValue)
                minValue = second->getData()[i];
            if(second->getData()[i] > maxValue)
                maxValue = second->getData()[i];

            rmsError = (first->getData()[i] - second->getData()[i]) * (first->getData()[i] - second->getData()[i]);

            if(rmsError > result)
            {
                result = rmsError;
            }
        }

        return result;
    }

    void ImageComparator::assertImagesAreEqual(std::string firstFilename, std::string secondFilename)
    {
        auto first = std::unique_ptr<Image>(ImageDeserializer::readImage(firstFilename));
        auto second = std::unique_ptr<Image>(ImageDeserializer::readImage(secondFilename));

        try
        {
            ImageComparator::assertImagesAreEqual(first.get(), second.get());
        }
        catch(const std::exception &e )
        {
            throw std::runtime_error("assertion between " + firstFilename + " and " + secondFilename + " failed. " + e.what());
        }
    }

    void ImageComparator::assertImagesAreEqual(Image *firstImage, Image *secondImage)
    {
        const float rms = getRMS(firstImage, secondImage);
        if (rms > 0.1f)
        {
            std::stringstream message;
            message << "Images are different, according to high cumulative RMS error " << rms << std::endl;
            throw std::runtime_error( message.str() );
        }

        const float maxPixelIntensity = std::fmaxf(firstImage->findMaxValue(), secondImage->findMaxValue());
        const float threshold = maxPixelIntensity * 0.00001f;
        const float maxError = getMaxError(firstImage, secondImage);
        if(maxError > threshold)
        {
            throw std::runtime_error("Images are different, according to large difference between two corresponding values. ");
        }
    }
}