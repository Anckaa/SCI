#pragma once

#include <stdexcept>
#include <stdint.h>

/// @brief Bit reader
class BitRead
{
public:
    using Data = const uint8_t;

public:
    /// @brief Constructor
    /// @param data_ptr - pointer to data
    BitRead(Data* &data_ptr) : m_data_ptr(data_ptr), m_offset(0) {}

    /// @brief Reading a bit (Big End)
    /// @return true if bit is 1.
    bool Bit();

    /// @brief Reading a bit (Little End)
    uint8_t BitLE();

    /// @brief Reading a some bits (Little End)
    /// @param length - amount of reading bits
    uint8_t Bits(uint8_t length);

    /// @brief Reading eath bits (Little End)
    uint8_t Byte();

    /// @brief Moving to next byte
    void Move() { ++m_data_ptr; m_offset = 0; }

private:
    static const uint8_t s_total_bit = 8;   // amount bits per byte

    // TODO: Replace the arrays to function with switch
    const uint8_t s_check_bitbe[s_total_bit] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };  // mask of reading bit (Big End)
    const uint8_t s_check_bitle[s_total_bit] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };  // mask of reading bit (Little End)
    const uint8_t s_mask_revers[s_total_bit] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };  // mask for checking zero

private:
    Data* &m_data_ptr;  // pointer to data
    uint8_t  m_offset;  // the current position
};

bool BitRead::Bit()
{
    if (m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    return (((*m_data_ptr) & s_check_bitbe[m_offset++]) != 0);
}

uint8_t BitRead::BitLE()
{
    if (m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    return (((*m_data_ptr) & s_check_bitle[m_offset++]) == 0) ? 0 : 1;
}

uint8_t BitRead::Bits(uint8_t length)
{
    if (!length || (length > s_total_bit))
    {
        throw std::runtime_error("length of bits is incorrect");
    }

    if (m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    m_offset += --length;
    if (m_offset < s_total_bit)
    {
        return ((*m_data_ptr) & s_mask_revers[m_offset]) >> ((m_offset++) - length);
    }

    uint8_t data_lo = *(m_data_ptr++) >> (m_offset - length);
    uint8_t data_hi = ((*m_data_ptr) & s_mask_revers[m_offset - s_total_bit]) << (s_total_bit - (m_offset - length));
    m_offset = m_offset - s_total_bit + 1;
    return data_hi | data_lo;
}

uint8_t BitRead::Byte()
{
    if (m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    if (!m_offset)
    {
        return *(m_data_ptr++);
    }

    uint8_t data_lo = *m_data_ptr >> m_offset;
    uint8_t data_hi = ((*(++m_data_ptr)) & s_mask_revers[m_offset - 1]) << (s_total_bit - m_offset);
    return data_hi | data_lo;
}
