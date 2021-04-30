
#include "libmmv/algorithm/RandomAlgorithms.h"
#include "libmmv/model/image/Image.h"
#include <random>
#include <functional>

namespace libmmv 
{
    void RandomAlgorithms::generateRandomNumbersFromMinusOneToOne(unsigned int amount, std::vector<float>& randomValues, unsigned int seed)
    {
        randomValues.resize(amount);
        std::mt19937 mt_rand(seed);
        auto real_rand = std::bind(std::uniform_real_distribution<float>(0, 1), mt_rand);
        for(unsigned int i = 0; i < amount; i++)
            randomValues[i] = real_rand();
    }

    std::vector<float> RandomAlgorithms::generateRandomNumbersFromMinusOneToOne(unsigned int amount, unsigned int seed)
    {
        std::vector<float> randomValues(amount);

        generateRandomNumbersFromMinusOneToOne(amount, randomValues);

        return randomValues;
    }

    void RandomAlgorithms::generateRandomNumbersFromZeroToMax(unsigned int amount, unsigned int maxValue, std::vector<unsigned int>& randomValues, unsigned int seed)
    {
        randomValues.resize(amount);
        std::mt19937 mt_rand(seed);
        auto int_rand = std::bind(std::uniform_int_distribution<unsigned int>(0, maxValue), mt_rand);
        for(unsigned int i = 0; i < amount; i++)
            randomValues[i] = (unsigned int)int_rand();
    }

    void RandomAlgorithms::generateRandomPointsFromZeroToMax(unsigned int amount, Vec2ui maxValue, std::vector<Vec2ui>& randomValues, unsigned int seed)
    {
        randomValues.resize(amount);

        std::mt19937 mt_rand(seed);
        auto int_randX = std::bind(std::uniform_int_distribution<unsigned int>(0, maxValue.x), mt_rand);
        auto int_randY = std::bind(std::uniform_int_distribution<unsigned int>(0, maxValue.y), mt_rand);

        for(unsigned int i = 0; i < amount; i++)
        {
            const unsigned int a = (unsigned int)int_randX();
            const unsigned int b = (unsigned int)int_randY();
            randomValues[i] = Vec2ui(a,b);
        }
    }

    std::vector<Vec3f> RandomAlgorithms::generateRandomUnitVectors(unsigned int amount, unsigned int seed)
    {
        std::vector<Vec3f> randomValues(amount);

        std::mt19937 mt_rand(seed);
        auto real_rand = std::bind(std::uniform_real_distribution<float>(0, 1), mt_rand);

        for(unsigned int i = 0; i < amount; i++)
            randomValues[i] = Vec3f(real_rand(), real_rand(), real_rand());

        return randomValues;
    }

    std::vector<unsigned int> RandomAlgorithms::generateRandomPermutationSequence(unsigned int sequenceLength, unsigned int seed)
    {
        std::random_device rd;
        std::mt19937 rfct(rd());

        std::vector<unsigned int> sequence(sequenceLength);
        for(unsigned int i = 0; i < sequenceLength; ++i) 
            sequence[i] = i;
        std::shuffle(sequence.begin(), sequence.end(), rfct);
        return sequence;
    }

    Image* RandomAlgorithms::generateRandomImage(unsigned int width, unsigned int height, unsigned int seed, float scalingFactor)
    {
        std::vector<float> randomValues;
        generateRandomNumbersFromMinusOneToOne(width * height, randomValues, seed);
        
        if (scalingFactor != 1.0)
            std::transform(randomValues.begin(), randomValues.end(), randomValues.begin(), std::bind(std::multiplies<float>(), scalingFactor, std::placeholders::_1));
    
        Image* randomImage = new Image(Vec2ui(width, height), &randomValues[0]);
        return randomImage;
    }

}