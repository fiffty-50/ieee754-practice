#ifndef EXERCISEGENERATOR_H
#define EXERCISEGENERATOR_H

#include"number.h"
#include<QRandomGenerator>

enum class Difficulty {easy, moderate, hard};

class ExerciseGenerator
{
public:
    ExerciseGenerator() = default;

    static Number generateDecimalNumber(Difficulty difficulty);

    /*!
     * \brief returns a randomised QBitArray of specified size
     */
    static QBitArray randomBitArray(int size);

private:
    /*!
     * \brief return a randomised bit
     */
    const static inline bool randomBit() {return QRandomGenerator::global()->bounded(2);}




};

#endif // EXERCISEGENERATOR_H
