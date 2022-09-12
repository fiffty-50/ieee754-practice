#ifndef EXERCISEGENERATOR_H
#define EXERCISEGENERATOR_H
#include "number.h"
#include "bitband.h"
#include "floatrepresentation.h"

enum class Difficulty {easy, moderate, hard};

class ExerciseGenerator
{
public:
    ExerciseGenerator() = default;

    enum Direction {FromBaseAddress, FromAliasAddress};

    static Number generateFloatingPointExercise(Difficulty difficulty);

    static FloatRepresentation generateFloatingPointExerciseNew(Difficulty difficulty);

    static BitBand generateBitBandExercise(Difficulty difficulty,
                                           BitBand::AddressingType addressing = BitBand::AddressingType::Byte,
                                           Direction direction = FromAliasAddress);
};

#endif // EXERCISEGENERATOR_H
