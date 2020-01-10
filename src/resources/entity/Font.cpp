#include <stdafx.h>
#include "Font.h"
#include <BitRead.h>

using namespace std;

Font::Character::Character(Character &&that)
    : m_font(that.m_font)
    , m_width(that.m_width)
    , m_height(that.m_height)
    , m_bitmap(std::move(that.m_bitmap))
{
    that.m_width = that.m_height = 0;
}

bool Font::Character::Import(ConstDataPtr &data, string &err)
{
    m_width  = *(data++);
    m_height = *(data++);

    if(m_height > m_font.Height())
    {
        err.assign("height of character is greater than height of font");
        return false;
    }

    try
    {
        m_bitmap.resize(m_width * m_height, false);
    }
    catch (bad_alloc exception)
    {
        err.assign(exception.what());
        return false;
    }

    BitRead reader(data);
    for(uint8_t i = 0, y = 0; y < m_height; ++y)
    {
        for(uint8_t x = 0; x < m_width; ++x, ++i)
            m_bitmap[i] = reader.Bit();
        reader.Move();
    }

    return true;
}

bool Font::Import(ConstDataPtr data, size_t size, string &err)
{
    Clear();

    if(!data)
    {
        err.assign("data is null");
        return false;
    }

    if(!size)
    {
        err.assign("data is empty");
        return false;
    }

    if(size < s_chars_offset_pos + 2)
    {
        err.assign("size of head is incorrect");
        return false;
    }

    ConstDataPtr data_end = data + size;
    uint16_t verify_bit = *data;
    if(verify_bit)
    {
        err.assign("begin data is not zero");
        return false;
    }

    uint16_t chars_total = *(data + s_chars_numers_pos);
    if(!chars_total)
    {
        err.assign("data has no characters");
        return false;
    }

    m_height = *(data + s_chars_height_pos);
    if(!m_height)
    {
        err.assign("characters has no weight");
        return false;
    }

    const uint16_t *offset_beg = reinterpret_cast<const uint16_t *>(data + s_chars_offset_pos);
    ConstDataPtr offset_end = data + s_chars_offset_pos + (chars_total * 2);
    if(offset_end > data_end)
    {
        err.assign("size of head is greater than size of data");
        return false;
    }

    try
    {
        m_chars.reserve(chars_total);
    }
    catch (bad_alloc exception)
    {
        err.assign(exception.what());
        return false;
    }

    Character character{ *this };
    ConstDataPtr char_data = data + (*offset_beg);
    for(uint16_t i = 0; i < chars_total; char_data = data + offset_beg[++i])
    {
        if(char_data < offset_end)
        {
            err.assign("offset the next charmap is out of range");
            return false;
        }

        if(!character.Import(char_data, err))
        {
            if(!err.empty()) err.insert(0, "Character: ");
            return false;
        }

        if(char_data > data_end)
        {
            err.assign("data of character is out of range");
            return false;
        }

        m_chars.push_back(move(character));
    }

    return true;
}

void Font::Clear()
{
    m_height = 0;
    m_chars.clear();
}

const Font::Character& Font::At(size_t index) const
{
    static const Font s_font_empty;
    static const Character s_char_empty(s_font_empty);
    return (index >= m_chars.size()) ? s_char_empty : m_chars[index];
}
