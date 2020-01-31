#include <stdafx.h>
#include "File.h"

namespace sci
{

namespace
{

/**
 * @brief Reading data from stream as number.
 *
 * @tparam T
 *  Type of output variable for reading.
 *
 * @param[in] stream
 *  Stream that has data for reading.
 * @param[in] amount_bytes
 *  Size of data per bytes.
 *
 * @return Reading data as numeric value.
 */
template <typename T>
T readNumeric(std::fstream &stream, int amount_bytes)
{
    T number;
    stream.read(reinterpret_cast<char *>(&number), amount_bytes);
    return number;
}

} // unnamed namespace

/**
 * @private The structure is implemented by pImpl paradigm.
 */
class File::FileImpl
{
    friend class File;

public:
    FileImpl(std::ios_base::openmode mode);
    bool Open(const char *filename, std::ostream &err) noexcept;
    bool Read(Data &buffer, Data::size_type size, std::ostream &err) noexcept;
    bool Close() noexcept;

private:
    const std::ios_base::openmode m_mode;   ///< @brief The current mode for working with file.
    std::fstream m_stream;                  ///< @brief The current file stream.
    std::string m_filename;                 ///< @brief The current filename.
};

File::FileImpl::FileImpl(std::ios_base::openmode mode)
    : m_mode{ mode }
{
    m_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
}

bool File::FileImpl::Open(const char *filename, std::ostream &err) noexcept
{
    Close();

    if (!filename || !*filename)
    {
        ERROR_TO_STREAM(err) << "Filename is empty.\n";
        return false;
    }

    if (m_stream.is_open())
    {
        if (m_filename.compare(filename) == 0)
        {
            return true;
        }

        ERROR_TO_STREAM(err) << "'" << m_filename.c_str() << "' is opened instead '" << filename << "'.\n";
        return false;
    }

    m_filename.assign(filename);
    try
    {
        m_stream.open(m_filename, m_mode);
    }
    catch (const std::ios_base::failure &exception)
    {
        ERROR_TO_STREAM(err) << "'" << filename << "' isn't opened: " << exception.what() << ".\n";
        return false;
    }

    return m_stream.operator bool();
}

bool File::FileImpl::Read(Data &buffer, Data::size_type size, std::ostream &err) noexcept
{
    if (!m_stream)
    {
        ERROR_TO_STREAM(err) << "File has not initiated.\n";
        return false;
    }

    try
    {
        buffer.resize(size);
    }
    catch (const std::bad_alloc &exception)
    {
        ERROR_TO_STREAM(err) << "The output buffer isn't preparing: " << exception.what() << ".\n";
        return false;
    }

    std::streamsize ss_size = static_cast<std::streamsize>(size);
    try
    {
        m_stream.read(reinterpret_cast<char *>(buffer.data()), ss_size);
        if(m_stream.gcount() != ss_size)
        {
            throw std::ios_base::failure("part of data isn't read"_s);
        }
    }
    catch (const std::ios_base::failure &exception)
    {
        ERROR_TO_STREAM(err) << exception.what() << ".\n";
        return false;
    }

    return true;
}

bool File::FileImpl::Close() noexcept
{
    m_filename.clear();
    if (m_stream.is_open())
    {
        try
        {
            m_stream.close();
        }
        catch (const std::ios_base::failure &/*exception*/)
        {
            // It's very sad, but in fact, file has closed.
            return false;
        }
    }

    return true;
}

File::File(std::ios_base::openmode mode)
    : m_impl{ new FileImpl{ mode } }
{
}

File::~File()
{
    Close();
    delete m_impl;
    m_impl = nullptr;
}

bool File::Open(const char *filename, std::ostream &err) noexcept
{
    return m_impl->Open(filename, err);
}

bool File::Close() noexcept
{
    return m_impl->Close();
}

std::ios_base::openmode File::Mode() const noexcept
{
    return m_impl->m_mode;
}

const char* File::Filename() const noexcept
{
    return m_impl->m_filename.c_str();
}

bool File::IsOpen() const noexcept
{
    return m_impl->m_stream.is_open();
}

bool File::IsEof() const noexcept
{
    return m_impl->m_stream.eof();
}

std::streampos File::Position()
{
    return m_impl->m_stream.tellg();
}

void File::Position(std::streampos pos)
{
    m_impl->m_stream.seekg(pos);
}

void File::Seek(std::streamoff offset)
{
    m_impl->m_stream.seekg(offset, std::ios_base::cur);
}

uint8_t File::Get()
{
    return static_cast<uint8_t>(m_impl->m_stream.get());
}

uint16_t File::Get16LE()
{
    return readNumeric<uint16_t>(m_impl->m_stream, 2);
}

uint32_t File::Get24LE()
{
    return readNumeric<uint32_t>(m_impl->m_stream, 3);
}

uint32_t File::Get32LE()
{
    return readNumeric<uint32_t>(m_impl->m_stream, 4);
}

bool File::Read(Data &buffer, Data::size_type size, std::ostream &err) noexcept
{
    return m_impl->Read(buffer, size, err);
}

} // sci
