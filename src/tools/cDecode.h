#pragma once

#include "BitRead.h"

class cDecode
{
public:
    static cDecode* Make(eDecodeType type);

public:
    virtual ~cDecode() {}

    virtual std::size_t Apply(const uint8_t *in_data, std::size_t in_size, uint8_t *out_data, std::size_t out_size, std::string &err) = 0;

protected:
    cDecode() {}

    bool Verify(const uint8_t *data, std::size_t data_size, std::size_t result_size, std::string &err);
};

class cDecodeExplode : public cDecode
{
public:
    cDecodeExplode(const cDecodeExplode &) = delete;
    cDecodeExplode(cDecodeExplode &&) = delete;
    cDecodeExplode& operator = (const cDecodeExplode &) = delete;
    cDecodeExplode&& operator = (const cDecodeExplode &&) = delete;

public:
    cDecodeExplode() {}

    virtual std::size_t Apply(const uint8_t *in_data, std::size_t in_size, uint8_t *out_data, std::size_t out_size, std::string &err) override;

private:
    static const uint8_t  s_bin_format   = 0;
    static const uint8_t  s_ascii_format = 1;
    static const uint8_t  s_huffman_bad  = 0xFF;
    static const uint16_t s_mantissa[];

private:
    static uint8_t Huffman1(BitRead &reader);
    static uint8_t Huffman2(BitRead &reader);
};
