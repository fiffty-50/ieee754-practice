#ifndef TOOLS_H
#define TOOLS_H
#include <QString>

namespace Tools {

enum Alignment {AlignLeft, AlignRight};
/*!
 * \brief splits a continuous string representing a binary number into groups of four
 */
const static inline QString toNibbles(QString binary_string, Alignment alignment = AlignRight)
{

    if (binary_string.isEmpty())
        return {};
    if (binary_string.length() < 5)
        return binary_string;

    const QLatin1Char SPACE = QLatin1Char(' ');
    QString nibbles;
    int leftovers = binary_string.length() % 4;
    QString leftover_string;

    switch (alignment) {
    case AlignLeft:
        if (leftovers) {
            leftover_string = binary_string.right(leftovers);
            binary_string.chop(leftovers);
        }
        //DEB << "leftover string: " << leftover_string;
        for (int i = 0; i < binary_string.length() / 4; i++)
            nibbles.append(binary_string.mid(4 * i, 4) + SPACE);

        if(leftovers) // append the leftovers
            nibbles.append(leftover_string);
        else // chop the trailing whitespace
            nibbles.chop(1);
        break;
    case AlignRight:
        if (leftovers) {
            leftover_string = binary_string.left(leftovers) + SPACE;
            binary_string.remove(0, leftovers);
        }

        for (int i = 0; i < binary_string.length() / 4; i++)
            nibbles.append(binary_string.mid(4 * i, 4) + SPACE);

        if(leftovers) { // prepend the leftovers
            nibbles.prepend(leftover_string);
            nibbles.chop(1);
        }
        else // chop the trailing whitespace
            nibbles.chop(1);
        break;
        //case AlignLeft:
        //    for (int i = 0; i < binary_string.length() / 4; i++)
        //        nibbles.append(binary_string.mid(4 * i, 4) + SPACE);

        //    if(leftovers) // append the leftovers
        //        nibbles.append(binary_string.right(leftovers));
        //    else // chop the trailing whitespace
        //        nibbles.chop(1);
        //    break;
        //case AlignRight:
        //    DEB << __PRETTY_FUNCTION__ << "### BROKEN. FIX.";
        //    for (int i = binary_string.length() / 4; i > 0 / 4; i--)
        //        nibbles.append(binary_string.mid(4 * i, 4) + SPACE);

        //    if(leftovers) { // prepend the leftovers
        //        nibbles.prepend(binary_string.left(leftovers));
        //        nibbles.chop(1);
        //    }
        //    else // chop the trailing whitespace
        //        nibbles.chop(1);
        //    break;
    default:
        break;
    }




    return nibbles;
}

/*!
 * \brief returns a string of a numbers binary representation, split into groups of four.
 */
const static inline QString toNibbles(int32_t number, Alignment alignment = AlignRight)
{
    QString binary_string = QString::number(number, 2);
    return toNibbles(binary_string, alignment);
}

/*!
 * \brief return the hexadecimal representation of an integer number.
 */
const static inline QString toHex(int32_t number)
{
    return QString::number(number, 16).toUpper();
}

/*!
 * \brief returns a string of the binary representation of an integer number
 */
const static inline QString toBitString(int32_t number)
{
    return QString::number(number, 2);
}


} // namespace Tools

#endif // TOOLS_H
