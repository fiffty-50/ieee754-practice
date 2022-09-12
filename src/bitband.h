#ifndef BITBAND_H
#define BITBAND_H

#include "src/debug.h"
#include "src/tools.h"
#include <QString>

/*!
 * \brief The BitBand class encapsulates the logic for solving bit-banding exercises.
 * The Cortex-M4 is taken as an example, i.e. there are two bit banding areas, one for data
 * (0x2000 0000 through 0x200F FFFF are mapped to 0x2200 0000 through 0x23ffffff),
 * and one for peripherals (0x4000 0000 through 0x400F FFFF are mapped to 0x4200 0000 through 0x42FF FFFF ).
 */
class BitBand
{
public:
    enum class AddressSpace {Data, Peripherals, OutOfRange};
    enum class AddressingType {Byte, Word};

    BitBand();
    BitBand(int bit_banding_alias_address, AddressingType addressing = AddressingType::Byte);
    BitBand(int bit_banding_address, int bit_position, AddressingType addressing = AddressingType::Byte);
    ~BitBand() = default;

    int aliasAddress() const;
    int baseAddress() const;
    int bitPos() const;

    int bitPosOffset() const;
    int baseAddressOffset() const;
    int aliasStartAddress() const;
    int addressSpaceStartAddress() const;
    AddressingType addressingType() const;
    AddressSpace addressSpace() const;

    operator QString(){
        QString addr;
        switch (m_addressing) {
        case AddressingType::Byte:
            addr = "Byte";
            break;
        case AddressingType::Word:
            addr = "Word";
            break;
        default:
            break;
        }
        DEB << "### Bit Band ###";
        DEB << "Addressing  : " << addr;
        DEB << "Base Address: " << Tools::toHex(m_baseAddress);
        DEB << "Bit Position: " << m_bitPos;
        DEB << "BBA Address : " << Tools::toHex(m_AliasAddress);
        return {};
    }

private:
    int m_AliasAddress = 0;
    int m_baseAddress = 0;
    int m_bitPos = 0;
    AddressingType m_addressing;
    AddressSpace m_addressSpace;

    const static int START_DATA  = 0x20000000;
    const static int START_PERI  = 0x40000000;
    const static int BB_RANGE    = 0xfffff;

    const static int BBA_OFFSET  = 0x02000000;
    const static int BBA_RANGE   = 0x1ffffff;

    const static int BYTE_MASK   = 0x1c;
    const static int WORD_MASK   = 0x7c;
    const static int BYTE_OFFSET = 0x1ffffe0;
    const static int WORD_OFFSET = 0x1ffff80;
};

#endif // BITBAND_H
