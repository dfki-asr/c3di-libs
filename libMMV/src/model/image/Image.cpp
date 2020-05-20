#include "stdafx.h"

#include <cassert>

#include "Image.h"

namespace ettention
{
    Image::Image(const Image* other)
        : resolution(other->resolution)
        , data(other->data)
    {
    }

    Image::Image(const Vec2ui& resolution, const float* initialData)
        : resolution(resolution)
    {
        if (resolution.x * resolution.y == 0)
        {
            std::stringstream description;
            description << "Invalid image size: " << resolution;
            throw std::domain_error( description.str() );
        }

		data.resize(resolution.x * resolution.y);

        if(initialData)
        {
            data.assign(initialData, initialData + getPixelCount());
        }
    }
 
    Image::~Image()
    {
    }

    const Vec2ui& Image::getResolution() const
    {
        return resolution;
    }

    std::size_t Image::getPixelCount() const
    {
        return data.size();
    }

    std::size_t Image::getByteWidth() const
    {
        return getPixelCount() * sizeof(float);
    }

    float* Image::getData()
    {
        return &data[0];
    }

    const float* Image::getData() const
    {
        return &data[0];
    }

    float* Image::getScanLineData(unsigned int y)
    {
        return &data[y * resolution.x];
    }

    const float* Image::getScanLineData(unsigned int y) const
    {
        return &data[y * resolution.x];
    }

    void Image::convertToLogarithmicScale()
    {
        std::transform(data.begin(), data.end(), data.begin(), (float (*)(float))&std::log);
    }

    void Image::convertToLinearScale()
    {
        std::transform(data.begin(), data.end(), data.begin(), (float(*)(float))&std::exp);
    }

    float Image::getPixel(int x, int y) const
    {
        return data[y * resolution.x + x];
    }

    void Image::setPixel(size_t x, size_t y, float value)
    {
        data[y * resolution.x + x] = value;
    }

    float Image::computeAverageValue() const
    {
        float sum = 0.0f;
        for (auto value : data)
            sum += value;
        return sum / (float) data.size();
    }

    float Image::findMinValue() const
    {
        return *std::min_element(data.begin(), data.end());
    }

    float Image::findMaxValue() const
    {
        return *std::max_element(data.begin(), data.end());
    }
}