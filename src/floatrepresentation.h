#ifndef FLOATREPRESENTATION_H
#define FLOATREPRESENTATION_H
#include <QBitArray>
#include "debug.h"


/*!
 * \brief The FloatRepresentation class encapsulates the different parts of an IEEE-754 Single Precision Floating Point Number.
 * The Elements are held individually, in order to make them more readily accessible. The sign bit is stored as a boolean, the
 * exponent and mantissa as Bit-Arrays. Some type casting happens inside the member functions.
 */
class FloatRepresentation {
public:
    FloatRepresentation();
    FloatRepresentation(bool sign, QBitArray exponent, QBitArray mantissa);

    enum Format {Binary, Nibbles, Decimal, Hexadecimal};

    static FloatRepresentation fromFloat(float value_in);
    static FloatRepresentation fromString(const QString &value_in);
    float toFloat() const;



    /*!
     * \brief return a QString of the floating point number
     */
    const QString toString(Format format) const;

    /*!
     * \brief return an unsigned 32-bit integer with the binary representation of the floating point number
     */
    uint32_t toUInt() const;

private:
    bool m_sign;
    QBitArray m_exponent;
    QBitArray m_mantissa;

    /*!
     * \brief return the Hexadecimal representation of the floating point number
     */
    const QString toHex() const;

    /*!
     * \brief convert to a string of the decimal number.
     */
    const QString toDecimalString() const;

public:
    operator QString(){ // For Debugging
        DEB << "Sign: " << m_sign;
        DEB << "Expo: " << m_exponent;
        DEB << "Mant: " << m_mantissa;
        return QString();
    };
};

#endif // FLOATREPRESENTATION_H
