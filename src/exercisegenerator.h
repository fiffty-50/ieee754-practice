#ifndef EXERCISEGENERATOR_H
#define EXERCISEGENERATOR_H
#include "number.h"
#include "bitband.h"

enum class Difficulty {easy, moderate, hard};

class ExerciseGenerator
{
public:
    ExerciseGenerator() = default;

    static Number generateFloatingPointExercise(Difficulty difficulty);

    static BitBand generateBitBandExercise(Difficulty difficulty);
};

#endif // EXERCISEGENERATOR_H
