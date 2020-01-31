#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace sci
{
/**
 * @defgroup SciTools SCI Tools
 * @brief The module provides tool set for working with SCI resources.
 *
 * There is have tools for working with files.
 * @{
 */

/**
 * @brief A file processing.
 *
 * A class provides a base management with files: to open text/binary files, to read data, to move into file and etc.
 */
class File
{
public:
    using Data = std::vector<uint8_t>;  ///< @brief A type for container of binary data that has been got from file.

public:
    /**
     * @brief Default constructor.
     *
     * @param[in] mode
     *  Type for file opening mode flags.
     */
    File(std::ios_base::openmode mode);

    /// @brief Default destructor.
    virtual ~File();

    File(const File &) = delete;                ///< @private using a method is denied
    File(File &&) = delete;                     ///< @private using a method is denied
    File& operator = (const File &) = delete;   ///< @private using a method is denied
    File&& operator = (const File &&) = delete; ///< @private using a method is denied

    /**
     * @brief Open the file.
     *
     * @param[in] filename
     *  c-string with path to file.
     * @param[out] err
     *  A standard stream that will be got messages if the method has error.
     *
     * @return true if file is opened successfully.
     */
    virtual bool Open(const char *filename, std::ostream &err) noexcept;

    /**
     * @brief Close the current file.
     *
     * @return true if file is closed successfully.
     */
    virtual bool Close() noexcept;

    /// @brief Getting the current mode.
    inline std::ios_base::openmode Mode() const noexcept;

    /**
     * @brief Getting the current filename.
     *
     * @note If filename isn`t set, method returns empty c-string.
     */
    inline const char* Filename() const noexcept;

    /**
     * @brief Getting status of file.
     *
     * @return true if file is open.
     */
    inline bool IsOpen() const noexcept;

    /**
     * @brief Getting flag about end of file.
     *
     * @return true if file has data for reading.
     */
    inline bool IsEof() const noexcept;

    /**
     * @brief Getting the current position in the file.
     *
     * @return Start position for processing.
     *
     * @throw std::ios_base::failure is thrown by method if position isn't available.
     */
    std::streampos Position();

    /**
     * @brief Setting the absolute position in the current file.
     *
     * @param[in] pos
     *  Value of the new position.
     *
     * @throw std::ios_base::failure is thrown by method if position isn't available.
     */
    void Position(std::streampos pos);

    /**
     * @brief Setting the relevate position in the current file.
     *
     * @param[in] offset
     *  Delta for jumping.
     *
     * @throw std::ios_base::failure is thrown by method if position isn't available.
     */
    void Seek(std::streamoff offset);

    /**
     * @brief Reading a byte from the file.
     *
     * @throw std::ios_base::failure is thrown by method if reading isn't available.
     */
    uint8_t Get();

    /**
     * @brief Reading two byte from file.
     *
     * @throw std::ios_base::failure is thrown by method if reading isn't available.
     */
    uint16_t Get16LE();

    /**
     * @brief Reading three byte from file.
     *
     * @return A double bytes, which has value in three low bytes.
     *
     * @throw std::ios_base::failure is thrown by method if reading isn't available.
     */
    uint32_t Get24LE();

    /**
     * @brief Getting four byte from file.
     *
     * @throw std::ios_base::failure is thrown by method if reading isn't available.
     */
    uint32_t Get32LE();

    /**
     * @brief Reading data from file.
     *
     * @param[out] buffer
     *  Container with reading data.
     * @param[in] size
     *  Size of the container.
     * @param[out] err
     *  A standard stream that will be got messages if the method has error.
     *
     * @return true if reading is finished successfully.
     */
    bool Read(Data &buffer, Data::size_type size, std::ostream &err) noexcept;

private:
    /**
     * @brief Support a pImpl paradigm.
     */
    struct FileImpl;
    FileImpl *m_impl;
};

///@} SciApi
} // sci
