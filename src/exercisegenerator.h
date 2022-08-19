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

    static Number generateFloatingPointExercise(Difficulty difficulty);

    static FloatRepresentation generateFloatingPointExerciseNew(Difficulty difficulty);

    static BitBand generateBitBandExercise(Difficulty difficulty, BitBand::AddressingType addressing = BitBand::AddressingType::Byte);
};

#endif // EXERCISEGENERATOR_H
