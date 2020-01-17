#pragma once

#include <Decode.h>
#include <File.h>
#include "Resource.h"

/// @brief Processing file with packed resources
class ResourcePackage : protected File
{
public:
    ResourcePackage(ResourcePackage &&) = delete;
    ResourcePackage(const ResourcePackage &) = delete;
    ResourcePackage& operator = (const ResourcePackage &) = delete;
    ResourcePackage&& operator = (const ResourcePackage &&) = delete;

public:
    /// @brief Constructors
    ResourcePackage() : File(std::ios_base::in | std::ios_base::binary) {}
// TODO: ???    ResourcePackage(std::ios_base::openmode mode) : File(mode) {}

    /// @brief share some methods of inherited class (@see File)
    /// @{
    const char* Filename() { return File::Filename(); }
    bool IsOpen()          { return File::IsOpen();   }
/*
    bool CopyTo(cResourcePackage &destination, std::string err) { return cFile::CopyTo(destination, err); }
    bool CopyTo(std::streampos position, cResourcePackage &destination, std::string err) { return cFile::CopyTo(position, destination, err); }
*/
    /// @}

    /// @brief Read base information of resource
    /// @param type         - type of resource
    /// @param id           - identifier of resource
    /// @param decode       - identifier of decoder
    /// @param pack_size    - size of packed data (actual size of data)
    /// @param real_size    - expected size of data after that will be unpacked
    /// @param err          - standard stream for output message about error
    /// @return true if reading is successed
    bool ReadHead(Resource::Type &type, Resource::Id &id,
                  Decode::Type &decode, Resource::Position &pack_size, Resource::Position &real_size, std::ostream &err) noexcept;

    /// @brief Read base information of resource
    /// @param type - type of resource
    /// @param id   - identifier of resource
    /// @param data - unpacked data of resource
    /// @param err  - standard stream for output message about error
    /// @return true if reading is successed
    bool ReadData(Resource::Type type, Resource::Id id, Resource::Position position, Resource::Data &data, std::ostream &err) noexcept;
};
