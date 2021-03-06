#include <stdafx.h>
#include "Decode.h"

using namespace std;

Decode* Decode::Make(Type type) noexcept
{
    switch(type)
    {
    case type_none:
    case type_null:
        return nullptr;
    case dcl_explode_1:
    case dcl_explode_2:
    case dcl_explode_3:
        return new (std::nothrow) DecodeExplode();
    }
}

bool Decode::Verify(const uint8_t *data, std::size_t data_size, std::size_t max_size, ostream &err) noexcept
{
    if (!data)
    {
        ERROR_TO_STREAM(err) << "decode error: compressing data of resource is null\n";
        return false;
    }

    if (!data_size)
    {
        ERROR_TO_STREAM(err) << "decode error: compressing data of resource is empty\n";
        return false;
    }

    if (data_size > max_size)
    {
        ERROR_TO_STREAM(err) << "decode error: uncompressing size of resource data is less that after compressing\n";
        return false;
    }

    return true;
}


std::size_t DecodeExplode::Apply(const uint8_t *in_data_ptr, std::size_t in_size, uint8_t *out_data_ptr, std::size_t out_max_size, std::ostream &err)
{
    if (!Verify(in_data_ptr, in_size, out_max_size, err))
    {
        return 0;
    }

    if (in_size < 3)
    {
        ERROR_TO_STREAM(err) << "input data for decode is incorrectly\n";
        return 0;
    }

    const uint8_t *terminator = in_data_ptr + in_size - 3;
    uint8_t        format_val = *in_data_ptr++;
    uint8_t        multiplier = *in_data_ptr++;

    if (format_val != s_bin_format)
    {
        ERROR_TO_STREAM(err) << "ascii format for decoding resource data doesn't support\n";
        return 0;
    }

    BitRead bit_reader(in_data_ptr);
    uint8_t  len_dat = 0, len_flt = 0;
    uint16_t len_val = 0;
    uint8_t  dist_lo = 0;
    uint16_t dist_hi = 0, dist_val = 0;
    std::size_t    i = 0;

    while (in_data_ptr < terminator && i < out_max_size)
    {
        if (!bit_reader.BitLE())
        {
            out_data_ptr[i++] = bit_reader.Byte();
            continue;
        }

        len_dat = Huffman1(bit_reader);
        if (len_dat == s_huffman_bad)
        {
            ERROR_TO_STREAM(err) << "decode error: value in huffman(1) is not defined\n";
            return 0;
        }

        if (len_dat > 7)
        {
            len_flt = len_dat - 7;
            if(len_flt >= s_mantissa_size)
            {
                ERROR_TO_STREAM(err) << "decode error: float party for define a length is incorrectly\n";
                return 0;
            }

            len_val = bit_reader.Bits(len_flt) + s_mantissa[len_flt] + 2;
        }
        else
        {
            len_val = len_dat + 2;
        }

        if(i + len_val > out_max_size)
        {
            ERROR_TO_STREAM(err) << "decode error: uncompressing size of resource data is greater than size of container\n";
            return 0;
        }

        dist_hi = Huffman2(bit_reader);
        if (dist_hi == s_huffman_bad)
        {
            ERROR_TO_STREAM(err) << "decode error: value in huffman(2) is not defined\n";
            return 0;
        }

        if (len_val == 2)
        {
            dist_hi = static_cast<uint16_t>(dist_hi << 2);
            dist_lo = bit_reader.Bits(2);
        }
        else
        {
            dist_hi = static_cast<uint16_t>(dist_hi << multiplier);
            dist_lo = bit_reader.Bits(multiplier);
        }

        dist_val = (dist_hi | dist_lo) + 1;
        if (dist_val > i)
        {
            ERROR_TO_STREAM(err) << "decode error: distance for coping bytes is out of range in container\n";
            return 0;
        }

        if(len_val > dist_val)
        {
            for(std::size_t len_cnt = 0, len_max = len_val / dist_val; len_cnt < len_max; ++len_cnt, len_val -= dist_val)
            {
                memcpy(&out_data_ptr[i], &out_data_ptr[i - dist_val], dist_val);
                i += dist_val;
            }
        }

        if(len_val)
        {
            memcpy(&out_data_ptr[i], &out_data_ptr[i - dist_val], len_val);
            i += len_val;
        }
    }

    if(in_data_ptr < terminator)
    {
        ERROR_TO_STREAM(err) << "decode error: uncompressing size of resource data is greater than size of container\n";
        return 0;
    }

    return i;
}

uint8_t DecodeExplode::Huffman1(BitRead &reader)
{
    // 1 + ...
    if(reader.BitLE())
        switch(reader.BitLE())
        {
            case 1: return 1;
            case 0: switch(reader.BitLE())
            {
                case 1: return 0;
                case 0: return 2;
            }
        }

    // 0 + ...
    switch(reader.BitLE())
    {
        case 1: switch(reader.BitLE())
        {
            case 1: return 3;
            case 0: switch(reader.BitLE())
            {
                case 1: return 4;
                case 0: return 5;
            }
        }
        case 0: switch(reader.BitLE())
        {
            case 1: switch(reader.BitLE())
            {
                case 1: return 6;
                case 0: switch(reader.BitLE())
                {
                    case 1: return 7;
                    case 0: return 8;
                }
            }
            case 0: switch(reader.BitLE())
            {
                case 1: switch(reader.BitLE())
                {
                    case 1: return 9;
                    case 0: return 10;
                }
                case 0: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: return 11;
                        case 0: return 12;
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: return 13;
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 14;
                            case 0: return 15;
                        }
                    }
                }
            }
        }
    }

    return s_huffman_bad;
}

uint8_t DecodeExplode::Huffman2(BitRead &reader)
{
    // 1 + ...
    if(reader.BitLE())
        switch(reader.BitLE())
        {
            case 1: return 0x00;
            case 0: switch(reader.BitLE())
            {
                case 1: switch(reader.BitLE())
                {
                    case 1: return 0x01;
                    case 0: return 0x02;
                }
                case 0: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: return 0x03;
                        case 0: return 0x04;
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: return 0x05;
                        case 0: return 0x06;
                    }
                }
            }
        }

    // 0 + ...
    switch(reader.BitLE())
    {
        case 1: switch(reader.BitLE())
        {
            case 1: switch(reader.BitLE())
            {
                case 1: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: return 0x07;
                        case 0: return 0x08;
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: return 0x09;
                        case 0: return 0x0a;
                    }
                }
                case 0: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: return 0x0b;
                        case 0: return 0x0c;
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: return 0x0d;
                        case 0: return 0x0e;
                    }
                }
            }
            case 0: switch(reader.BitLE())
            {
                case 1: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: return 0x0f;
                        case 0: return 0x10;
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: return 0x11;
                        case 0: return 0x12;
                    }
                }
                case 0: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: return 0x13;
                        case 0: return 0x14;
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: return 0x15;
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x16;
                            case 0: return 0x17;
                        }
                    }
                }
            }
        }
        case 0: switch(reader.BitLE())
        {
            case 1: switch(reader.BitLE())
            {
                case 1: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: return 0x18;
                            case 0: return 0x19;
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x1a;
                            case 0: return 0x1b;
                        }
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: return 0x1c;
                            case 0: return 0x1d;
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x1e;
                            case 0: return 0x1f;
                        }
                    }
                }
                case 0: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: return 0x20;
                            case 0: return 0x21;
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x22;
                            case 0: return 0x23;
                        }
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: return 0x24;
                            case 0: return 0x25;
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x26;
                            case 0: return 0x27;
                        }
                    }
                }
            }
            case 0: switch(reader.BitLE())
            {
                case 1: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: return 0x28;
                            case 0: return 0x29;
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x2a;
                            case 0: return 0x2b;
                        }
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: return 0x2c;
                            case 0: return 0x2d;
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: return 0x2e;
                            case 0: return 0x2f;
                        }
                    }
                }
                case 0: switch(reader.BitLE())
                {
                    case 1: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: switch(reader.BitLE())
                            {
                                case 1: return 0x30;
                                case 0: return 0x31;
                            }
                            case 0: switch(reader.BitLE())
                            {
                                case 1: return 0x32;
                                case 0: return 0x33;
                            }
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: switch(reader.BitLE())
                            {
                                case 1: return 0x34;
                                case 0: return 0x35;
                            }
                            case 0: switch(reader.BitLE())
                            {
                                case 1: return 0x36;
                                case 0: return 0x37;
                            }
                        }
                    }
                    case 0: switch(reader.BitLE())
                    {
                        case 1: switch(reader.BitLE())
                        {
                            case 1: switch(reader.BitLE())
                            {
                                case 1: return 0x38;
                                case 0: return 0x39;
                            }
                            case 0: switch(reader.BitLE())
                            {
                                case 1: return 0x3a;
                                case 0: return 0x3b;
                            }
                        }
                        case 0: switch(reader.BitLE())
                        {
                            case 1: switch(reader.BitLE())
                            {
                                case 1: return 0x3c;
                                case 0: return 0x3d;
                            }
                            case 0: switch(reader.BitLE())
                            {
                                case 1: return 0x3e;
                                case 0: return 0x3f;
                            }
                        }
                    }
                }
            }
        }
    }

    return s_huffman_bad;
}
