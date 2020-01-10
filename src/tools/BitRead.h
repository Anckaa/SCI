#pragma once

class BitRead
{
public:
    using ConstData    = const uint8_t;
    using ConstDataPtr = const uint8_t *;

public:
    BitRead(ConstDataPtr &data_ptr) : m_data_ptr(data_ptr), m_offset(0) {}

    inline bool Bit();
    inline bool BitLE();
    inline uint8_t Bits(uint8_t length);
    inline uint8_t Byte();

    inline void Move() { ++m_data_ptr; m_offset = 0; }

private:
    static const uint8_t s_total_bit      = 8;
           const uint8_t s_check_bitbe[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
           const uint8_t s_check_bitle[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
           const uint8_t s_mask_revers[8] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };

private:
    ConstDataPtr &m_data_ptr;
    uint8_t       m_offset;
};

inline bool BitRead::Bit()
{
    if(m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    return (((*m_data_ptr) & s_check_bitbe[m_offset++]) != 0);
}

inline bool BitRead::BitLE()
{
    if(m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    return bool (((*m_data_ptr) & s_check_bitle[m_offset++]) != 0);
}

inline uint8_t BitRead::Bits(uint8_t length)
{
    if(!length || (length > s_total_bit))
        throw std::runtime_error("length of bits is incorrect");

    if(m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    m_offset += --length;
    if(m_offset < s_total_bit)
        return ((*m_data_ptr) & s_mask_revers[m_offset]) >> ((m_offset++) - length);

    uint8_t data_lo = *(m_data_ptr++) >> (m_offset - length);
    uint8_t data_hi = ((*m_data_ptr) & s_mask_revers[m_offset - s_total_bit]) << (s_total_bit - (m_offset - length));
    m_offset = m_offset - s_total_bit + 1;
    return data_hi | data_lo;
}

inline uint8_t BitRead::Byte()
{
    if(m_offset == s_total_bit)
    {
        ++m_data_ptr;
        m_offset = 0;
    }

    if(!m_offset)
        return *(m_data_ptr++);

    uint8_t data_lo = *m_data_ptr >> m_offset;
    uint8_t data_hi = ((*(++m_data_ptr)) & s_mask_revers[m_offset - 1]) << (s_total_bit - m_offset);
    return data_hi | data_lo;
}
