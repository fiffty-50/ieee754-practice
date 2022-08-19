#ifndef BITBAND_H
#define BITBAND_H

#include <QString>

/*!
 * \brief The BitBand class encapsulates the logic for solving bit-banding exercises.
 * The Cortex-M4 is taken as an example, i.e. there are two bit banding areas, one for data
 * (0x2000 0000 through 0x200F FFFF are mapped to 0x2200 0000 through 0x23ffffff),
 * and one for peripherals(0x4000 0000 through 0x400F FFFF are mapped to 0x4200 0000 through 0x42FF FFFF ).
 */
class BitBand
{
public:
    enum Range {data, peripherals};
    BitBand() = delete;
    BitBand(int bit_banding_alias_address);
    BitBand(int bit_banding_address, int bit_position);

    int bbAliasAddress() const;
    int bbAddress() const;
    int bitPos() const;

    enum Direction {FromBBA, FromBitPos};
    QString getStep(Direction direction, int step_number);

private:
    int m_bbAliasAddress = 0;
    int m_bbAddress = 0;
    int m_bitPos = 0;

    const static int m_BB_start_address_data = 0x20000000;
    const static int m_BB_end_address_data = 0x200fffff;
    const static int m_BBA_start_address_data = 0x22000000;
    const static int m_BBA_end_address_data = 0x23ffffff;

    const static int m_BB_start_address_peripherals = 0x40000000;
    const static int m_BB_end_address_peripherals = 0x400fffff;
    const static int m_BBA_start_address_peripherals = 0x42000000;
    const static int m_BBA_end_address_peripherals = 0x43ffffff;



};

#endif // BITBAND_H
