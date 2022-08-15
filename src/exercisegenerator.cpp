#include "exercisegenerator.h"
#include <QString>

Number ExerciseGenerator::generateDecimalNumber(Difficulty difficulty)
{
    //ExerciseGenerator eg;
    QBitArray integer;
    QBitArray decimals;
    switch (difficulty) {
    case Difficulty::easy:
        integer = randomBitArray(6);
        decimals = randomBitArray(3);
        break;
    case Difficulty::moderate:
        integer = randomBitArray(10);
        decimals = randomBitArray(4);
        break;
    case Difficulty::hard:
        integer = randomBitArray(16);
        decimals = randomBitArray(5);
        break;
    }

    bool sign = randomBit();
    return Number(sign, integer, decimals);
}

QBitArray ExerciseGenerator::randomBitArray(int size)
{
    QBitArray array(size);
    for (int i = 0; i < size; i++)
        array[i] = randomBit();

    // do not return an array consisting of only zeroes
    while (!array.count(true)) {
        for (int i = 0; i < size; i++)
            array[i] = randomBit();
    }

    return array;
}
