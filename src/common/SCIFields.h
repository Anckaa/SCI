#pragma once

using tResourcePosition = uint32_t;
using tResourceOffset   = int64_t;

static const tResourcePosition s_resource_position_none = -1;

// TODO: ???
using tDecodeType = uint8_t;
enum class eDecodeType : tDecodeType
{
    none          = 0x00,

    dcl_explode_1 = 0x12,
    dcl_explode_2 = 0x13,
    dcl_explode_3 = 0x14,

    first = dcl_explode_1,
    last  = dcl_explode_3,
    null  = 0xFF
};

static tDecodeType s_decode_type_none  = static_cast<tDecodeType>(eDecodeType::none);
static tDecodeType s_decode_type_first = static_cast<tDecodeType>(eDecodeType::first);
static tDecodeType s_decode_type_last  = static_cast<tDecodeType>(eDecodeType::last);
static tDecodeType s_decode_type_null  = static_cast<tDecodeType>(eDecodeType::null);
