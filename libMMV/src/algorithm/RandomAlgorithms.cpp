#include <random>
#include "libmmv/algorithm/RandomAlgorithms.h"
#include "libmmv/model/image/Image.h"

namespace libmmv 
{
    void RandomAlgorithms::generateRandomNumbersFromMinusOneToOne(unsigned int amount, std::vector<float>& randomValues, unsigned int seed)
    {
        randomValues.resize(amount);
        std::default_random_engine generator( seed );
        std::uniform_real_distribution<float> distribution( -1.0f, 1.0f );

        for (unsigned int i = 0; i < amount; i++)
            randomValues[i] = distribution( generator );
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
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<unsigned int> distribution( 0, maxValue );

        for (unsigned int i = 0; i < amount; i++)
            randomValues[i] = distribution(generator);
    }

    void RandomAlgorithms::generateRandomPointsFromZeroToMax(unsigned int amount, Vec2ui maxValue, std::vector<Vec2ui>& randomValues, unsigned int seed)
    {
        randomValues.resize(amount);

        std::default_random_engine generator(seed);
        std::uniform_int_distribution<unsigned int> distributionA( 0, maxValue.x );
        std::uniform_int_distribution<unsigned int> distributionB( 0, maxValue.y );

        for(unsigned int i = 0; i < amount; i++)
        {
            const unsigned int a = distributionA(generator);
            const unsigned int b = distributionB(generator);
            randomValues[i] = Vec2ui(a,b);
        }
    }

	std::vector<Vec3ui> RandomAlgorithms::generateRandomPointsFromZeroToMax(unsigned int amount, Vec3ui maxValue, unsigned int seed)
	{
		std::vector<Vec3ui> randomValues;
		randomValues.resize(amount);

		std::default_random_engine generator(seed);
		std::uniform_int_distribution<unsigned int> distributionA(0, maxValue.x);
		std::uniform_int_distribution<unsigned int> distributionB(0, maxValue.y);
		std::uniform_int_distribution<unsigned int> distributionC(0, maxValue.z);

		for (unsigned int i = 0; i < amount; i++)
		{
			const unsigned int a = distributionA(generator);
			const unsigned int b = distributionB(generator);
			const unsigned int c = distributionC(generator);
			randomValues[i] = Vec3ui(a, b, c);
		}
		return randomValues;
	}

	std::vector<Vec3ui> RandomAlgorithms::generateRandomPointsFromZeroToOne(unsigned int amount, unsigned int seed)
	{
		return generateRandomPointsFromZeroToMax(amount, Vec3ui(1, 1, 1), seed);
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
            for (size_t index = 0; index < randomValues.size(); index++)
                randomValues[index] *= scalingFactor;
    
        Image* randomImage = new Image(Vec2ui(width, height), &randomValues[0]);
        return randomImage;
    }

}