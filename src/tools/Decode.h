#pragma once

#include "BitRead.h"
#include <iostream>

/// @brief Decoder for binary data
class Decode
{
public:
    /// @brief Type of decode
    enum Type
    {
        type_none = 0x00,

        dcl_explode_1 = 0x12,
        dcl_explode_2 = 0x13,
        dcl_explode_3 = 0x14,

        type_first = dcl_explode_1,
        type_last  = dcl_explode_3,
        type_null  = 0xFF
    };

public:
    /// @brief Factory method
    /// @param type - type of decode
    /// @return Pointer to decoder
    static Decode* Make(Type type) noexcept;

public:
    /// @brief Destructor
    virtual ~Decode() = default;

    /// @brief Apply this decode for the binary data
    /// @param in_data_ptr  - the input data
    /// @param in_size      - size of input data
    /// @param out_data_ptr - the output data
    /// @param out_max_size - max size of output data
    /// @param err          - standard stream for output message about error
    /// @return size of output data (in bytes)
    virtual std::size_t Apply(const uint8_t *in_data_ptr, std::size_t in_size, uint8_t *out_data_ptr, std::size_t out_max_size, std::ostream &err) = 0;

protected:
    /// @brief Protected Constructor
    Decode() = default;

    /// @brief Apply Check the input data
    /// @param data         - the input data
    /// @param data_size    - size of input data
    /// @param max_size     - size of output data
    /// @param err          - standard stream for output message about error
    /// @return true if verify success
    bool Verify(const uint8_t *data, std::size_t data_size, std::size_t max_size, std::ostream &err) noexcept;
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
    virtual std::size_t Apply(const uint8_t *in_data_ptr, std::size_t in_size, uint8_t *out_data_ptr, std::size_t out_max_size, std::ostream &err) override;
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
    /// @return decoded value
    static uint8_t Huffman1(BitRead &reader);

    /// @brief Huffman alghoritm
    /// @param reader - bit reader
    /// @return decoded value
    static uint8_t Huffman2(BitRead &reader);
};
