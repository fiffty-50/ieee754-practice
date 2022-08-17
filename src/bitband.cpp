#include "bitband.h"
#include <QRandomGenerator>

BitBand::BitBand(int bit_banding_alias_address)
    : m_bbAliasAddress(bit_banding_alias_address) {

    if ((m_bbAliasAddress >= m_BBA_start_address_data) && (m_bbAliasAddress <= m_BBA_end_address_data)) { // Data Bit Band
        m_bitPos = (m_bbAliasAddress & 0x1c) >> 2;
        m_bbAddress = ((m_bbAliasAddress & 0x1ffffe0) >> 5) | m_BB_start_address_data;
    } else if ((m_bbAliasAddress >= m_BBA_start_address_peripherals) && (m_bbAliasAddress <= m_BBA_end_address_peripherals)) { // Peripherals Bit Band
        m_bitPos = (m_bbAliasAddress & 0x1c) >> 2;
        m_bbAddress = ((m_bbAliasAddress & 0x1ffffe0) >> 5) | m_BB_start_address_peripherals;
    } else { // Value out of range
        m_bitPos = 0;
        m_bbAddress = 0;
        m_bbAliasAddress = 0;
    }
}

BitBand::BitBand(int bit_banding_address, int bit_position)
    : m_bbAddress(bit_banding_address), m_bitPos(bit_position)
{
    if ((m_bbAddress >= m_BB_start_address_data) && (m_bbAddress <= m_BB_end_address_data)) { // Data Bit Band
        m_bbAliasAddress = (m_BBA_start_address_data + ((m_bbAddress - m_BB_start_address_data) << 5)) + (m_bitPos << 2);
    } else if ((m_bbAddress >= m_BB_start_address_peripherals) && (m_bbAddress <= m_BB_end_address_peripherals)) { // Peripherals Bit Band
        m_bbAliasAddress = (m_BBA_start_address_peripherals + ((m_bbAddress - m_BB_start_address_peripherals) << 5)) + (m_bitPos << 2);
    } else { // Value out of range
        m_bitPos = 0;
        m_bbAddress = 0;
        m_bbAliasAddress = 0;
    }
}

int BitBand::bbAliasAddress() const
{
    return m_bbAliasAddress;
}

int BitBand::bbAddress() const
{
    return m_bbAddress;
}

int BitBand::bitPos() const
{
    return m_bitPos;
}
