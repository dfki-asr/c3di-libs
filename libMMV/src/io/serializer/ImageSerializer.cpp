#include <cstring>
#include <fstream>
#include <FreeImagePlus.h>
#include "libmmv/io/serializer/ImageSerializer.h"
#include "libmmv/model/image/Image.h"

namespace libmmv
{

    ImageSerializer::ImageSerializer()
    {
    }


    ImageSerializer::~ImageSerializer()
    {
    }

    void ImageSerializer::writeImage(std::string filenameTrunk, const Image* image, ImageSerializer::ImageFormat format)
    {
        fipImage fimg(FIT_FLOAT, image->getResolution().x, image->getResolution().y, 32);

        for(unsigned int y = 0; y < fimg.getHeight(); y++)
        {
            memcpy(fimg.getScanLine(y), image->getScanLineData(y), fimg.getWidth() * sizeof(float));
        }

        std::string filename = getFileName(filenameTrunk, format);
        if(!fimg.isValid())
        {
            throw std::runtime_error(("Image to save to " + filename + " is invalid!").c_str());
        }

        if(isFormat8bit(format))
        {
            fimg.convertToType(FIT_BITMAP);
        }

        fimg.flipVertical();
        if(!fimg.save(filename.c_str()))
        {
            throw std::ios_base::failure( "Saving of image to %1% failed!" + filename );
        }
    }

    std::string ImageSerializer::getFileName(std::string filenameTrunk, ImageSerializer::ImageFormat format)
    {
        if(format == TIFF_GRAY_32)
            return filenameTrunk + ".tif";
        if(format == BPM_GRAY_8)
            return filenameTrunk + ".bpm";
        if(format == PNG_GRAY_8)
            return filenameTrunk + ".png";
        if(format == PNG_24)
            return filenameTrunk + ".png";
        throw std::runtime_error("image format not supported");
    }

    std::string ImageSerializer::constructFilename(std::string& filenameTrunk)
    {
        return filenameTrunk + ".pgm";
    }

    std::vector<unsigned char> ImageSerializer::scaleDataToBytes(float* data, size_t width, size_t height, float dataMinValue, float dataMaxValue)
    {
        size_t numPixels = width * height;
        std::vector<unsigned char> byteBuffer;
        byteBuffer.reserve(numPixels);

        float dataRangeInverse = 1.0f / (dataMaxValue - dataMinValue);
        for(unsigned int i = 0; i < numPixels; i++)
        {
            byteBuffer.push_back(static_cast<unsigned char>(((data[i] - dataMinValue) * dataRangeInverse) * 255.f));
        }

        return byteBuffer;
    }

    std::string ImageSerializer::constructGrayscalePPMHeader(size_t width, size_t height)
    {
        std::stringstream header;
        header << "P5 " << width << " " << height << " 255" << std::endl;
        return header.str();
    }

    std::pair<float, float> ImageSerializer::computeExtremalValuesOfFloatBuffer(float* data, unsigned int size)
    {
        std::pair<float, float> extrema(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
        for(unsigned int i = 0; i < size; i++)
        {
            if(data[i] < extrema.first)
                extrema.first = data[i];
            if(data[i] > extrema.second)
                extrema.second = data[i];
        }
        return extrema;
    }

    void ImageSerializer::writePPM(const std::string& filename, const std::string& imageHeader, const std::vector<unsigned char>& imageData)
    {
        std::ofstream ppmFileStream(filename.c_str(), std::ofstream::binary);
        if(!ppmFileStream.good())
        {
            std::string errorMessage = "Unable to open file for PPM serialization " + filename;
            throw std::runtime_error(errorMessage.c_str());
        }

        std::ostreambuf_iterator<char> ppmFileIterator(ppmFileStream);

        std::copy(imageHeader.begin(), imageHeader.end(), ppmFileIterator);
        std::copy(imageData.begin(), imageData.end(), ppmFileIterator);

        ppmFileStream.close();
    }

    bool ImageSerializer::isFormat8bit(ImageFormat format)
    {
        return ((format == BPM_GRAY_8) || (format == PNG_GRAY_8));
    }

    bool ImageSerializer::isFormatPNG(ImageFormat format)
    {
        return ((format == PNG_GRAY_8) || (format == PNG_24));
    }
}