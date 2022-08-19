#ifndef NUMBER_H
#define NUMBER_H
#include <QBitArray>
#include <QString>
#include "debug.h"

/*!
 * \brief Represents a decimal number in base 2, e.g. 1011.011. Contains logic for conversion to and from IEEE-754 single precision floating point representation.
 */
class Number {
public:
    Number() = default;
    Number(bool sign_, const QBitArray &integer_, const QBitArray &decimals_)
        : sign(sign_), integer(integer_), decimals(decimals_) {}

    enum class Format {Binary, Decimal, Hexadecimal};

    /*!
     * \brief return the encapsulated number as a string representation
     * \return
     */
    QString toString(Format format);

    /*!
     * \brief return a (optionally styled) string of the nibbles making up the binary representation of self
     */
    QString toNibbles(bool styled = false);

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
    QString biasedExponent(Format format = Format::Binary);

    /*!
     * \brief return the sign bit
     */
    bool getSign() const;

    /*!
     * \brief obtain the bias
     */
    int getBias();

    /*!
     * \brief create a Number object from a double input value.
     * \return
     */
    static Number fromFloat(float value_in);

    operator QString(){
        DEB << "Sign: " << sign;
        DEB << "integer part: " << integer;
        DEB << "decimal part: " << decimals;
        return QString();
    };





private:
    bool sign = false;
    int shift = 0;
    QBitArray integer = QBitArray();
    QBitArray decimals = QBitArray();

    /*!
     * \brief takes a string of bits e.g. "1011" and converts it to hexadecimal
     */
    QString toHexString();

    /*!
     * \brief returns a double of the value encapsulated in self
     */
    QString toDecimalString();

    /*!
     * \brief return a string of the binary representation of self
     */
    QString toBinaryString();
};

#endif // NUMBER_H
