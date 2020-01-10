#pragma once

class File
{
public:
    using Data = std::vector<uint8_t>;

public:
    File(const File &) = delete;
    File(File &&) = delete;
    File& operator = (const File &) = delete;
    File&& operator = (const File &&) = delete;

public:
    File(std::ios_base::openmode mode) : m_mode(mode) {}
    virtual ~File() { Close(); }

    virtual bool Open(const char *filename, std::ostream &err);
    virtual void Close();

    inline std::ios_base::openmode Mode() const { return m_mode;             }
    inline const char* Filename() const         { return m_filename.c_str(); }
    inline bool IsOpen() const                  { return m_stream.is_open(); }

    inline std::streampos Position() const   { return m_stream.tellg();                    }
    inline bool IsEof() const                { return m_stream.eof();                      }
    inline void Position(std::streampos pos) { m_stream.seekg(pos);                        }
    inline void Seek(std::streamoff offset)  { m_stream.seekg(offset, std::ios_base::cur); }

    inline uint8_t  Get()     { return m_stream.get(); }
    inline uint16_t Get16LE() { uint16_t value; m_stream.read(reinterpret_cast<char *>(&value), 2); return value; }
    inline uint32_t Get24LE() { uint32_t value; m_stream.read(reinterpret_cast<char *>(&value), 3); return value; }
    inline uint32_t Get32LE() { uint32_t value; m_stream.read(reinterpret_cast<char *>(&value), 4); return value; }
    bool Read(Data &buffer, Data::size_type size, std::ostream &err);
/*
    bool CopyTo(File &destination, std::ostream &err);
    bool CopyTo(std::streampos position, File &destination, std::ostream &err);
*/
protected:
    Data m_buffer;

private:
    static const Data::size_type s_buffer_size = 64 * 1024;

private:
    const std::ios_base::openmode m_mode;
    mutable std::fstream          m_stream;
    std::string                   m_filename;
};
