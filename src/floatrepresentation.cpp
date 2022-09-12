#include "floatrepresentation.h"

FloatRepresentation::FloatRepresentation()
{
    m_sign = 0;
    m_exponent = QBitArray();
    m_mantissa = QBitArray();
}

FloatRepresentation::FloatRepresentation(bool sign, QBitArray exponent, QBitArray mantissa)
    : m_sign(sign), m_exponent(exponent), m_mantissa(mantissa) {
    if (m_exponent.size() != 8 | m_mantissa.size() != 23) {
        m_sign = 0;
        m_exponent = QBitArray();
        m_mantissa = QBitArray();
        DEB << "Invalid Input";
    }
}

FloatRepresentation FloatRepresentation::fromFloat(float value_in)
{
    DEB << __PRETTY_FUNCTION__ << "Float in: " << value_in;
    // reinterpret as uint to get bits
    unsigned int bits = *reinterpret_cast<int*>(&value_in);

    // get sign bit
    unsigned int sign_mask      = 0x80000000;
    bool sign = bits & sign_mask;

    // get exponent bits
    unsigned int exponent_mask  = 0xFF000000;
    unsigned int exponent = ((bits << 1) & exponent_mask) >> 24;
    QBitArray exponent_array(8);
    for (int i = 0; i < 8; i++){
        unsigned int mask = 0x80;
        exponent_array[i] = mask & exponent;
        exponent = exponent << 1;
    }

    // get mantissa bits
    unsigned int mantissa = reinterpret_cast<unsigned int&>(value_in) & (((unsigned int)1 << 24) - 1);
    QBitArray mantissa_array(23);
    for (int i = 0; i < 23; i++){
        unsigned int mask = 0x400000;
        mantissa_array[i] = mask & mantissa;
        mantissa = mantissa << 1;
    }

    return FloatRepresentation(sign, exponent_array, mantissa_array);
}

FloatRepresentation FloatRepresentation::fromString(const QString &value_in)
{
    //DEB << __PRETTY_FUNCTION__ << "Number in" << value_in;
    bool ok;
    uint32_t bits = value_in.toUInt(&ok, 2);
    // get sign bit
    unsigned int sign_mask      = 0x80000000;
    bool sign = bits & sign_mask;

    // get exponent bits
    unsigned int exponent_mask  = 0xFF000000;
    unsigned int exponent = ((bits << 1) & exponent_mask) >> 24;
    QBitArray exponent_array(8);
    for (int i = 0; i < 8; i++){
        unsigned int mask = 0x80;
        exponent_array[i] = mask & exponent;
        exponent = exponent << 1;
    }

    // get mantissa bits
    float number_as_float = reinterpret_cast<float&>(bits);
    unsigned int mantissa = reinterpret_cast<unsigned int&>(number_as_float) & (((unsigned int)1 << 24) - 1);
    QBitArray mantissa_array(23);
    for (int i = 0; i < 23; i++){
        unsigned int mask = 0x400000;
        mantissa_array[i] = mask & mantissa;
        mantissa = mantissa << 1;
    }
    //DEB << __PRETTY_FUNCTION__ << "Number out: " << sign << exponent_array << mantissa_array;
    return FloatRepresentation(sign, exponent_array, mantissa_array);
}

float FloatRepresentation::toFloat() const
{
    //DEB << "Rounding errors may occur...";
    // convert Bit String to UInt
    bool ok;
    qint32 number = toString(Binary).toUInt(&ok, 2);

    if(!ok)
        return 0;

    // reinterpret
    float number_out = *reinterpret_cast<float*>(&number);
    return number_out;
}

const QString FloatRepresentation::toHex() const
{
    return QLatin1String("0x") + QString::number(toUInt(), 16);
}

const QString FloatRepresentation::toString(Format format) const
{
    // create a String of all the bits
    QString bit_string;
    m_sign ? bit_string.append(QLatin1Char('1'))
           : bit_string.append(QLatin1Char('0'));

    // add the exponent and mantissa bits
    for (int i = 0; i < m_exponent.size(); i++)
        bit_string.append(QString::number(m_exponent[i]));
    for (int i = 0; i< m_mantissa.size(); i++)
        bit_string.append(QString::number(m_mantissa[i]));

    //DEB << __PRETTY_FUNCTION__ << "Bits: " << bit_string;

    // Prepare some switch variables and return the requested format
    bool ok;
    float float_number;
    QString nibbles;

    switch (format) {
    case Format::Binary:
        return bit_string;
        break;
    case Format::Nibbles:
        for (int i = 0; i < 8; i++)
            nibbles.append(bit_string.mid(4 * i, 4) + QLatin1Char(' '));
        nibbles.chop(1);
        return nibbles;
        break;
    case Format::Decimal:
        float_number = this->toFloat();
        return QString::number(float_number);
        break;
    case Format::Hexadecimal:
        return toHex();
        break;
    default:
        return bit_string;
        break;
    }
}

uint32_t FloatRepresentation::toUInt() const
{
    // convert Bit String to UInt
    bool ok;
    uint32_t number = toString(Binary).toUInt(&ok, 2);

    if(!ok)
        return 0;
    else
        return number;

}
