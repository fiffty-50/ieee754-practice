#include "bitband.h"
#include "src/debug.h"
#include "src/tools.h"
#include <QRandomGenerator>

BitBand::BitBand()
{
    m_bitPos = 0;
    m_baseAddress = 0;
    m_AliasAddress = 0;
    m_addressSpace = AddressSpace::OutOfRange;
}

BitBand::BitBand(int bit_banding_alias_address, AddressingType addressing)
    : m_AliasAddress(bit_banding_alias_address), m_addressing(addressing) {

    if((m_AliasAddress > START_PERI + BBA_OFFSET + BBA_RANGE) | (m_AliasAddress < START_DATA + BBA_OFFSET)) { // out of range
        m_bitPos = 0;
        m_baseAddress = 0;
        m_AliasAddress = 0;
        m_addressSpace = AddressSpace::OutOfRange;
        DEB << __PRETTY_FUNCTION__ << "Input Address out of range: " << Tools::toHex(bit_banding_alias_address);
        return;
    }

    switch (addressing) {
    case AddressingType::Byte:
        if(m_AliasAddress >= START_PERI + BBA_OFFSET) { // AddressSpace::Peripherals
            m_bitPos = (m_AliasAddress & BYTE_MASK) >> 2;
            m_baseAddress = ((m_AliasAddress & BYTE_OFFSET) >> 5) | (START_PERI);
            m_addressSpace = AddressSpace::Peripherals;
            return;
        }

        if(m_AliasAddress >= START_DATA + BBA_OFFSET) { // Data
            m_bitPos = (m_AliasAddress & BYTE_MASK) >> 2;
            m_baseAddress = ((m_AliasAddress & BYTE_OFFSET) >> 5) | (START_DATA);
            m_addressSpace = AddressSpace::Data;
            return;
        }

        if((m_AliasAddress > START_DATA + BBA_OFFSET + BBA_RANGE)) { // out of range
            m_bitPos = 0;
            m_baseAddress = 0;
            m_AliasAddress = 0;
            m_addressSpace = AddressSpace::OutOfRange;
            DEB << __PRETTY_FUNCTION__ << "Input Address out of range: " << Tools::toHex(bit_banding_alias_address);
            return;
        }
        break;
    case AddressingType::Word:
        if(m_AliasAddress >= START_PERI + BBA_OFFSET) { // Peripherals
            m_bitPos = (m_AliasAddress & WORD_MASK) >> 2;
            m_baseAddress = ((m_AliasAddress & WORD_OFFSET) >> 5) | (START_PERI);
            m_addressSpace = AddressSpace::Peripherals;
            return;
        }

        if(m_AliasAddress >= START_DATA + BBA_OFFSET) { // Data
            m_bitPos = (m_AliasAddress & WORD_MASK) >> 2;
            m_baseAddress = ((m_AliasAddress & WORD_OFFSET) >> 5) | (START_DATA);
            m_addressSpace = AddressSpace::Data;
            return;
        }

        if((m_AliasAddress > START_DATA + BBA_OFFSET + BBA_RANGE)) { // out of range
            m_bitPos = 0;
            m_baseAddress = 0;
            m_AliasAddress = 0;
            m_addressSpace = AddressSpace::OutOfRange;
            DEB << __PRETTY_FUNCTION__ << "Input Address out of range: " << Tools::toHex(bit_banding_alias_address);
            return;
        }
        break;
    default:
        break;
    }
}

BitBand::BitBand(int bit_banding_address, int bit_position, AddressingType addressing)
    : m_baseAddress(bit_banding_address), m_bitPos(bit_position), m_addressing(addressing)
{
    if(m_baseAddress > START_PERI + BB_RANGE | m_baseAddress < START_DATA) { // out of range
        m_bitPos = 0;
        m_baseAddress = 0;
        m_AliasAddress = 0;
        m_addressSpace = AddressSpace::OutOfRange;
        DEB << __PRETTY_FUNCTION__ << "Input Address out of range:" << Tools::toHex(bit_banding_address);
        return;
    }

    if(m_baseAddress >= START_PERI) { // AddressSpace::Peripherals
        m_AliasAddress = ((START_PERI + BBA_OFFSET) + ((m_baseAddress - START_PERI) << 5)) + (m_bitPos << 2);
        m_addressSpace = AddressSpace::Peripherals;
        DEB << __PRETTY_FUNCTION__ << "Constructing Object in Peripheral Range.";
        return;
    }

    if(m_baseAddress >= START_DATA) { // Data
        DEB << "Data In:" << Tools::toHex(bit_banding_address) << "Pos" << bit_position;
        m_AliasAddress = ((START_DATA + BBA_OFFSET) + ((m_baseAddress - START_DATA) << 5)) + (m_bitPos << 2);
        m_addressSpace = AddressSpace::Data;
        DEB << __PRETTY_FUNCTION__ << "Constructing Object in Data Range.";
        DEB << "Constructed  : ";
        DEB << "Base Address: " << Tools::toHex(m_baseAddress);
        DEB << "Bit Position: " << m_bitPos;
        DEB << "BBA Address : " << Tools::toHex(m_AliasAddress);
        return;
    }

    if(m_baseAddress > START_DATA + BB_RANGE) { // out of range
        m_bitPos = 0;
        m_baseAddress = 0;
        m_AliasAddress = 0;
        m_addressSpace = AddressSpace::OutOfRange;
        DEB << __PRETTY_FUNCTION__ << "Input Address out of range:" << Tools::toHex(bit_banding_address);
        return;
    }
}

int BitBand::BitBand::aliasAddress() const
{
    return m_AliasAddress;
}

int BitBand::baseAddress() const
{
    return m_baseAddress;
}

int BitBand::bitPos() const
{
    return m_bitPos;
}

BitBand::AddressingType BitBand::addressingType() const
{
    return m_addressing;
}

BitBand::AddressSpace BitBand::addressSpace() const
{
    return m_addressSpace;
}

int BitBand::bitPosOffset() const
{
    switch (m_addressing) {
    case AddressingType::Byte:
        return BYTE_MASK;
        break;
    case AddressingType::Word:
        return WORD_MASK;
        break;
    default:
        return BYTE_MASK;
        break;
    }
}

int BitBand::baseAddressOffset() const
{
    switch (m_addressing) {
    case AddressingType::Byte:
        return BYTE_OFFSET;
        break;
    case AddressingType::Word:
        return WORD_OFFSET;
        break;
    default:
        return BYTE_OFFSET;
        break;
    }
}

int BitBand::aliasStartAddress() const{
    switch (m_addressSpace) {
    case AddressSpace::Data:
        return START_DATA + BBA_OFFSET;
        break;
    case AddressSpace::Peripherals:
        return START_PERI + BBA_OFFSET;
        break;
    default:
        return 0;
        break;
    }
}

int BitBand::addressSpaceStartAddress() const{
    switch (m_addressSpace) {
    case AddressSpace::Data:
        return START_DATA;
        break;
    case AddressSpace::Peripherals:
        return START_PERI;
        break;
    default:
        return 0;
        break;
    }
}
