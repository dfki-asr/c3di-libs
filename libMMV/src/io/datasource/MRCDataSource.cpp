#include "libmmv/io/datasource/MRCDataSource.h"
#include "libmmv/model/image/Image.h"
#include "libmmv/io/datasource/ImageStackDataSource.h"

namespace libmmv
{
    MRCDataSource::MRCDataSource()
    {
    }

    MRCDataSource::MRCDataSource(const std::filesystem::path& stackFilePath, bool logaritmizeData)
        : stackFilePath(stackFilePath)
        , logaritmizeData(logaritmizeData)
    {
        init();
    }

    MRCDataSource::MRCDataSource(const std::filesystem::path& stackFilePath, const std::filesystem::path& tiltFilePath, bool logaritmizeData)
        : stackFilePath(stackFilePath)
        , logaritmizeData(logaritmizeData)
    {
        init();
    }

    MRCDataSource::~MRCDataSource()
    {
        if(file.good())
        {
            file.close();
        }
    }

    const char* MRCDataSource::getName() const
    {
        return "MRCDataSource";
    }

    libmmv::HyperStackIndex MRCDataSource::firstIndex() const
    {
        return HyperStackIndex(0);
    }

    libmmv::HyperStackIndex MRCDataSource::lastIndex() const
    {
        return HyperStackIndex(numberOfProjections - 1);
    }

    Image* MRCDataSource::loadProjectionImage(const HyperStackIndex& index)
    {
        if (index.getNumberOfDimensions() != 1)
        {
            throw std::runtime_error("MRC datasource only supports indices of dimension one");
        }

        Image* img = CachingImageStackDataSource::loadProjectionImage(index);
        if (img != nullptr) {
            return img;
        }

        std::vector<float> data(resolution.x * resolution.y);
        readImageData(&data[0], index[0]);
        img = new Image(resolution, &data[0]);
        cacheProjectionImage(index, img);
        return img;
    }

    void MRCDataSource::readImageData(float* dest, unsigned int projectionIndex)
    {
        switch(mrcHeader.mode)
        {
        case 0:
            this->readSlice<unsigned char>(dest, projectionIndex, -1.0f);
            break;
        case 1:
            this->readSlice<short>(dest, projectionIndex, dataMin - 1.0f);
            break;
        case 2: 
            this->readSlice<float>(dest, projectionIndex, dataMin - 1.0f);
            break;
        case 6:
            this->readSlice<unsigned short>(dest, projectionIndex, -1.0f);
            break;
        default:
            throw std::ios_base::failure("Format of the MRC stack is either unknown or not supported");
            break;
        }
    }

    template <typename _T>
    float MRCDataSource::convertValue(_T rawValue, float minValue) const
    {
        float rawFloat = (float)rawValue;
        if(logaritmizeData)
        {
            return logf(dataMax / (rawFloat - minValue));
        }
        else
        {
            return rawFloat;
        }
    }

    template <typename _T>
    void MRCDataSource::readSlice(float* dest, unsigned int projectionIndex, float minValue)
    {
        auto pixels = resolution.x * resolution.y;
        _T* data = new _T[pixels];
        file.seekg(sizeof(MRCHeader) + mrcHeader.extra + (projectionIndex * pixels) * sizeof(_T));
        file.read((char*)data, pixels * sizeof(_T));
        for(int i = 0; i < mrcHeader.ny; i++)
        {
            for(int j = 0; j < mrcHeader.nx; j++)
            {
                float value = convertValue(data[i * mrcHeader.nx + j], minValue);
                dest[i*mrcHeader.nx + j] = value;
            }
        }
        delete[] data;
    }

    template<typename _T>
    void MRCDataSource::getDataRange()
    {
        dataMax = -std::numeric_limits<float>::infinity();
        dataMin = std::numeric_limits<float>::infinity();
        std::size_t voxelCount = (std::size_t)mrcHeader.ny * (std::size_t)mrcHeader.nx * (std::size_t)mrcHeader.nz;
        for(std::size_t i = 0; i < voxelCount; ++i)
        {
            _T value;
            file.read((char*)&value, sizeof(_T));
            if(!file.good())
            {
				throw std::ios_base::failure("Error reading " + std::to_string(voxelCount) + " voxel value(s) from " + stackFilePath.string() + "!");
            }
            dataMax = std::max((float)value, dataMax);
            dataMin = std::min((float)value, dataMin);
        }
    }

    void MRCDataSource::init()
    {
        std::string path = stackFilePath.string();
        file.open(path, std::ios::binary);
        if(!file.good())
        {
            throw std::ios_base::failure( "Could not open MRC stack" + stackFilePath.string() );
        }
        file.read((char*)&mrcHeader, sizeof(MRCHeader));
        file.ignore(mrcHeader.extra);

        numberOfProjections = mrcHeader.nz;

        switch(mrcHeader.mode)
        {
        case 0:
            this->getDataRange<unsigned char>();
            break;
        case 1:
            this->getDataRange<signed short>();
            break;
        case 2:
            this->getDataRange<float>();
            break;
        case 6:
            this->getDataRange<unsigned short>();
            break;
        default:
            std::stringstream message;
            message << "Format of the MRC stack is either unknown or not supported (mode " << mrcHeader.mode << ")";
            throw std::ios_base::failure( message.str() );
        }

        if(mrcHeader.mode == 1 || mrcHeader.mode == 2)
        {
            dataMax = dataMax - dataMin + 1.0f;
        }
        else if(mrcHeader.mode == 0 || mrcHeader.mode == 6)
        {
            dataMax = dataMax + 1.0f;
        }

        resolution = Vec2ui(mrcHeader.nx, mrcHeader.ny);
    }

    unsigned int MRCDataSource::getNumberOfProjections() const
    {
        return numberOfProjections;
    }

    Vec2ui MRCDataSource::getResolution() const
    {
        return resolution;
    }

}