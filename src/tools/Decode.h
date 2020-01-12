#pragma once

#include "BitRead.h"

/// @brief Decoder of binary data
class Decode
{
public:
    /// @brief Type of decode
    enum eType
    {
        none          = 0x00,

        dcl_explode_1 = 0x12,
        dcl_explode_2 = 0x13,
        dcl_explode_3 = 0x14,

        first = dcl_explode_1,
        last  = dcl_explode_3,
        null  = 0xFF
    };

public:
    /// @brief Factory mathod
    /// @param type - type of decode
    /// @return Pointer to decoder
    static Decode* Make(eType type) noexcept;

public:
    /// @brief Destructor
    virtual ~Decode() = default;

    /// @brief Apply this decode for the binary data
    /// @param in_data  - the input data
    /// @param in_size  - size of input data
    /// @param out_data - the output data
    /// @param out_size - size of output data
    /// @param err      - standard stream for output message about error
    /// @return real size of output data (in bytes)
    virtual std::size_t Apply(const uint8_t *in_data, std::size_t in_size, uint8_t *out_data, std::size_t out_size, std::ostream &err) = 0;

protected:
    /// @brief Protected Constructor
    Decode() = default;

    /// @brief Apply Check the input data
    /// @param data         - the input data
    /// @param size         - size of input data
    /// @param result_data  - size of output data
    /// @param err          - standard stream for output message about error
    /// @return true if verify success
    bool Verify(const uint8_t *data, std::size_t data_size, std::size_t result_size, std::ostream &err) noexcept;
};

/// @brief Explode decoder
class DecodeExplode : public Decode
{
public:
    DecodeExplode(const DecodeExplode &) = delete;
    DecodeExplode(DecodeExplode &&) = delete;
    DecodeExplode& operator = (const DecodeExplode &) = delete;
    DecodeExplode&& operator = (const DecodeExplode &&) = delete;

public:
    /// @brief Public Constructor
    DecodeExplode() = default;

    /// @implements Decode
    /// @{
    virtual std::size_t Apply(const uint8_t *in_data, std::size_t in_size, uint8_t *out_data, std::size_t out_size, std::ostream &err) override;
    /// @}

private:
    static const uint8_t  s_bin_format   = 0;       // flag of binary data
    static const uint8_t  s_ascii_format = 1;       // flag of text data
    static const uint8_t  s_huffman_bad  = 0xFF;    // falg of bad decode

    static const uint8_t s_mantissa_size = 9;
    const uint16_t s_mantissa[s_mantissa_size] = { 0x07, 0x08, 0x0a, 0x0e, 0x16, 0x26, 0x46, 0x86, 0x106 };

private:
    /// @brief Huffman alghoritm
    /// @param reader - bit reader
    /// @return decoding byte
    static uint8_t Huffman1(BitRead &reader);

    /// @brief Huffman alghoritm
    /// @param reader - bit reader
    /// @return decoding byte
    static uint8_t Huffman2(BitRead &reader);
};
