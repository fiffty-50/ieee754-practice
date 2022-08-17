#ifndef RANDOMBITS_H
#define RANDOMBITS_H
#include <QBitArray>
#include <QRandomGenerator>


class RandomBits
{
public:
    RandomBits() = delete;

    /*!
     * \brief return a randomised bit
     */
    const static inline bool randomBit() {return QRandomGenerator::global()->bounded(2);}

    /*!
     * \brief returns a randomised QBitArray of specified size
     */
    const static QBitArray randomBitArray(int size)
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


};

#endif // RANDOMBITS_H
