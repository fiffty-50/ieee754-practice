#ifndef NUMBER_H
#define NUMBER_H
#include <QBitArray>
#include <QString>

/*!
 * \brief Represents a decimal number in base 2, e.g. 1011.011
 */
class Number {
public:
    Number() = default;
    Number(bool sign_, const QBitArray &integer_, const QBitArray &decimals_)
        : sign(sign_), integer(integer_), decimals(decimals_) {}

    /*!
     * \brief returns a string of the absolute binary representation of number
     */
    QString absoluteBinary();

    /*!
     * \brief returns the binary as a normalised binary number with only one leading 1
     */
    QString normalisedBinary();

    /*!
     * \brief return the mantissa normalised to 23 digits
     */
    QString mantissa();

    /*!
     * \brief return the biased exponent in a binary String
     */
    QString biasedExponent();

    /*!
     * \brief return the sign bit
     */
    bool getSign() const;

    /*!
     * \brief return a string of the binary representation of self
     */
    QString toBinaryString();

    /*!
     * \brief return a list of the nibbles making up the binary representation of self
     */
    QStringList toNibbles();

    /*!
     * \brief takes a string of bits e.g. "1011" and converts it to hexadecimal
     */
    QString toHex();

    /*!
     * \brief returns a double of the value encapsulated in self
     */
    QString toDecimalString();


    int getShift();

private:
    bool sign = false;
    int shift = 0;
    QBitArray integer = QBitArray();
    QBitArray decimals = QBitArray();

};

#endif // NUMBER_H
