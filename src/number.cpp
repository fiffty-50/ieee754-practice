#include "number.h"
#include "src/tools.h"
#include <math.h>
#include <QRegularExpression>
#include <QDebug>



int Number::getBias()
{
    if (shift == 0)
        normalisedBinary();
    return shift;
}

Number Number::fromFloat(float value_in)
{
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

    DEB << "Sign:" << sign << " - " << QString::number(sign, 2);
    DEB << "Exponent:" << exponent << " - " << exponent_array;
    DEB << "mantissa:" << mantissa << " - " << mantissa_array;

    return Number(sign, exponent_array, mantissa_array);

}

QString Number::toString(Format format)
{
    switch (format) {
    case Format::Binary:
        return QString::number(sign) + biasedExponent() + mantissa();
        break;
    case Format::Decimal:
        return toDecimalString();
        break;
    case Format::Hexadecimal:
        return toHexString();
        break;
    default:
        return {};
    }
}



QString Number::mantissa(bool styled)
{
    QString mantissa = normalisedBinary();
    mantissa.remove(0,2);
    while (mantissa.length() != 23)
        mantissa.append(QLatin1Char('0'));

    if (!styled)
        return mantissa;
    else
        return Tools::toNibbles(QString("<font color='orange'>%1</font>").arg(mantissa), Tools::AlignRight);
}

QString Number::biasedExponent(Format format)
{
    if (shift == 0)
        normalisedBinary();

    int biased_exponent = 127 + shift;

    switch (format) {
    case Format::Binary:
        return (QString::number(biased_exponent, 2));
        break;
    case Format::Decimal:
        return (QString::number(biased_exponent, 10));
        break;
    case Format::Hexadecimal:
        return (QString::number(biased_exponent, 16));
        break;
    default:
        return (QString::number(biased_exponent, 2));
        break;
    }

}

bool Number::getSign() const
{
    return sign;
}

QString Number::toNibbles(bool styled)
{
    QString binary_string = toBinaryString();


    QString out;
    if (!styled) {
        for (int i = 0; i < 9; i++)
            out.append(binary_string.mid(4 * i, 4) + QLatin1Char(' '));
    } else {
        out.append(QLatin1String("<font color='blue'>") + binary_string[0] + QLatin1String("</font><font color='orange'>") + binary_string.mid(1,3)); // first nibble
        out.append(QLatin1Char(' ') + binary_string.mid(4,4));
        out.append(QLatin1Char(' ') + binary_string[8] + QLatin1String("</font><font color='green'>") + binary_string.mid(9,3));
        for (int i = 3; i < 9; i++)
            out.append(binary_string.mid(4 * i, 4) + QLatin1Char(' '));
        out.append(QLatin1String("</font>"));
    }
    return out;
}

QString Number::absoluteBinary()
{
    if (integer.size() == 0) {
        return QString("0");
    }

    QString binary_string;


    for (int i = 0; i < integer.size(); i++) {
        binary_string.append(QString::number(integer[i]));
    }

    binary_string.append(QLatin1Char('.'));

    for (int i = 0; i < decimals.size(); i++) {
        binary_string.append(QString::number(decimals[i]));
    }

    // Remove leading zeroes
    static QRegularExpression re("^[0]*");
    binary_string.remove(re);

    return binary_string;
}

QString Number::normalisedBinary()
{
    QString absolute_binary_string = absoluteBinary();
    // get the decimal separator position
    int original_decimal_position = absolute_binary_string.indexOf(QLatin1Char('.'));
    // get the position of the first '1'
    int leftmost_one = absolute_binary_string.indexOf(QLatin1Char('1'));
    // determine how many positions the decimal point needs to be shifted
    shift = (original_decimal_position - (leftmost_one + 1));
    // edit the binary string
    absolute_binary_string.remove(0, leftmost_one);
    absolute_binary_string.remove(QLatin1Char('.'));
    absolute_binary_string.insert(1, QLatin1Char('.'));

    return absolute_binary_string;
}

QString Number::toBinaryString()
{
    return QString::number(sign) + biasedExponent() + mantissa();
}

QString Number::toDecimalString()
{
    DEB << "integer array: " << integer;
    DEB << "decimal array: " << decimals;
    qint64 int_out = 0;
    // Convert integer part
    int j = 0;
    for (int i = integer.size() -1; i >= 0; i--) {
        if (integer[i])
            int_out += pow(2, j);
        j++;
    }

    double dec_out = 0;
    // convert decimal part
    j = 1;
    for (int i = 0; i < decimals.size(); i ++) {
        if (decimals[i]) {
            dec_out += pow(2, -j);
        }
        j++;
    }

    QString out = QString::number(int_out) + QLatin1Char('.') + QString::number(dec_out).remove(0,2);
    if (sign)
        out = out.prepend(QLatin1Char('-'));

    DEB << "Out: " << out;

    return out;
}

QString Number::toHexString()
{
    QString binary_string = toBinaryString();
    bool ok;
    auto value = binary_string.toULong(&ok, 2);  //convert Bit String in base 2 to int
    if (!ok)
        return QString();

    QString hex_string = QString::number(value, 16);  //Create number string with base 16
    return hex_string.toUpper();
}

