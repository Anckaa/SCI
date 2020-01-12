#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>

/// @brief Base processing of file
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
    /// @brief Constructor
    /// @param mode - of openning file
    File(std::ios_base::openmode mode) : m_mode(mode) {}

    /// @brief Destructor
    virtual ~File() { Close(); }

    /// @brief Open the file
    /// @param filename - c-string with path to file
    /// @param err      - standard stream for output message about error
    /// @return true if file was open success
    virtual bool Open(const char *filename, std::ostream &err);

    /// @brief Close the current file
    virtual void Close();

    /// @brief Getting the current mode
    std::ios_base::openmode Mode() const { return m_mode; }

    /// @brief Getting the current filename
    /// @note will be returned empty c-string if filename isn`t  set
    const char* Filename() const { return m_filename.c_str(); }

    /// @brief Getting status of file
    /// @return true if file is open
    bool IsOpen() const { return m_stream.is_open(); }

    /// @brief Getting the current position
    std::streampos Position() { return m_stream.tellg(); }

    /// @brief Getting flag about end of file
    bool IsEof() const { return m_stream.eof(); }

    /// @brief Setting the absolute position
    /// @param pos - value the new position
    void Position(std::streampos pos) { m_stream.seekg(pos); }

    /// @brief Setting the relevate position
    /// @param offset - delta for jump
    void Seek(std::streamoff offset)  { m_stream.seekg(offset, std::ios_base::cur); }

    /// @brief Getting a byte from file
    uint8_t  Get() { return m_stream.get(); }

    /// @brief Getting two byte from file
    uint16_t Get16LE() { uint16_t value; m_stream.read(reinterpret_cast<char *>(&value), 2); return value; }

    /// @brief Getting three byte from file
    uint32_t Get24LE() { uint32_t value; m_stream.read(reinterpret_cast<char *>(&value), 3); return value; }

    /// @brief Getting four byte from file
    uint32_t Get32LE() { uint32_t value; m_stream.read(reinterpret_cast<char *>(&value), 4); return value; }

    /// @brief Read data from file
    /// @param buffer   - container for data
    /// @param size     - size of container
    /// @param err      - standard stream for output message about error
    /// @return true if reading was success
    bool Read(Data &buffer, Data::size_type size, std::ostream &err) noexcept;

/*
    bool CopyTo(File &destination, std::ostream &err);
    bool CopyTo(std::streampos position, File &destination, std::ostream &err);
protected:
    Data m_buffer;

private:
    static const Data::size_type s_buffer_size = 64 * 1024;
*/

private:
    const std::ios_base::openmode m_mode;       // the current mode for working with file
    std::fstream                  m_stream;     // the current file stream
    std::string                   m_filename;   // the current filename
};
