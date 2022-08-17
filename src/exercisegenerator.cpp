#include "exercisegenerator.h"
#include "RandomBits.h"

Number ExerciseGenerator::generateFloatingPointExercise(Difficulty difficulty)
{
    //ExerciseGenerator eg;
    QBitArray integer;
    QBitArray decimals;
    switch (difficulty) {
    case Difficulty::easy:
        integer  = RandomBits::randomBitArray(6);
        decimals = RandomBits::randomBitArray(3);
        break;
    case Difficulty::moderate:
        integer  = RandomBits::randomBitArray(10);
        decimals = RandomBits::randomBitArray(4);
        break;
    case Difficulty::hard:
        integer  = RandomBits::randomBitArray(16);
        decimals = RandomBits::randomBitArray(5);
        break;
    }

    bool sign = RandomBits::randomBit();
    return Number(sign, integer, decimals);
}

BitBand ExerciseGenerator::generateBitBandExercise(Difficulty difficulty)
{
    // Easy :  Bitpos 0  <= n <= 4      BB-Addr. 0x2000 0000 through 0x2000 0F00
    // Medium: Bitpos 5  <= n <= 10     BB-Addr. 0x2000 0000 through 0x2000 F000
    // Hard:   Bitpos 11 <= n <= 31     BB-Addr. 0x2000 0000 through 0x200F FFFF
    int bitBandAddress, bitPos;
    switch (difficulty) {
    case Difficulty::easy:
        bitBandAddress = QRandomGenerator::global()->bounded(0x20000000, 0x20000F00);
        bitPos =         QRandomGenerator::global()->bounded(4);
        break;
    case Difficulty::moderate:
        bitBandAddress = QRandomGenerator::global()->bounded(0x20000F00, 0x2000F000);
        bitPos =         QRandomGenerator::global()->bounded(5, 10);
        break;
    case Difficulty::hard:
        bitBandAddress = QRandomGenerator::global()->bounded(0x2000F000, 0x200FFFFF);
        bitPos =         QRandomGenerator::global()->bounded(11, 31);
        break;
    default:
        bitBandAddress = QRandomGenerator::global()->bounded(0x20000000, 0x20000F00);
        bitPos =         QRandomGenerator::global()->bounded(4);
        break;
    }

    return BitBand(bitBandAddress, bitPos);
}
