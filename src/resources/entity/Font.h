#pragma once

class Font
{
public:
    using ConstDataPtr = const uint8_t *;

public:
    class Character
    {
        friend class Font;

    public:
        // TODO: implement allocator into Font for blocking the segmentation of memory
        using Bitmap = std::vector<bool>;

    public:
        Character(const Character &) = delete;
        Character& operator = (const Character &) = delete;
        Character&& operator = (const Character &&) = delete;

    public:
        Character(const Font &font) : m_font(font), m_width(0), m_height(0), m_bitmap(0) {}
        Character(Character &&that);

        inline bool     Empty() const     { return m_bitmap.empty(); }
        inline uint16_t Width() const     { return m_width;          }
        inline uint16_t Height() const    { return m_height;         }
        inline const Bitmap& Data() const { return m_bitmap;         }

    private:
        const Font &m_font;
        uint16_t    m_width;
        uint16_t    m_height;
        Bitmap      m_bitmap;

        bool Import(ConstDataPtr &data, std::string &err);
    };

public:
    Font(const Font &) = delete;
    Font(Font &&) = delete;
    Font& operator = (const Font &) = delete;
    Font&& operator = (Font &&) = delete;

public:
    Font() : m_height(0) {}

    void Clear();
    bool Import(ConstDataPtr data, std::size_t size, std::string &err);

    inline bool        Empty() const  { return m_chars.empty(); }
    inline std::size_t Total() const  { return m_chars.size();  }
    inline uint16_t    Height() const { return m_height;        }

    const Character& At(std::size_t index) const;
    inline const Character& operator[](std::size_t index) const { return m_chars[index]; }

private:
    static const std::size_t s_chars_numers_pos = 2;
    static const std::size_t s_chars_height_pos = 4;
    static const std::size_t s_chars_offset_pos = 6;

private:
    using CharacterArray = std::vector<Character>;

private:
    CharacterArray m_chars;
    uint16_t       m_height;
};
