#pragma once

#include <climits>
#include <iostream>
#include <stdint.h>
#include <vector>

/// @brief Base access to resource
class Resource
{
public:
    /// @brief Type of resource
    enum Type
    {
        view     = 0x80,
        pic      = 0x81,
        script   = 0x82,
        text     = 0x83,
        sound    = 0x84,
        object   = 0x85,
        vocab    = 0x86,
        font     = 0x87,
        cursor   = 0x88,
        patch    = 0x89,
        bitmap   = 0x8a,
        palette  = 0x8b,
        cd_audio = 0x8c,
        audio    = 0x8d,
        sync     = 0x8c,
        message  = 0x8f,
        map      = 0x90,
        dump     = 0x91,

        type_first = view,
        type_last  = dump,
        type_none  = 0xFF
    };

    using Id = uint16_t;
    using Data = std::vector<uint8_t>;

    using Package = uint8_t;
    using Position = uint32_t;

// TODO: !!! Look for access to type
private:
    using Offset = int64_t;

public:
    static const auto s_id_none = std::numeric_limits<Id>::max();
    static const auto s_package_none = std::numeric_limits<Package>::max();
    static const auto s_position_none = std::numeric_limits<Position>::max();

public:
    /// @brief Destructor
    virtual ~Resource() = default;

    /// @brief Init package container
    /// @param package_amount   - amount of packages
    /// @param err              - standard stream for output message about error
    /// @return true if initialization was successed
    virtual bool PreparePackages(Package package_amount, std::ostream &err) = 0;

    /// @brief Open the package
    /// @param package_index    - index of package
    /// @param filename         - name of package's file
    /// @param err              - standard stream for output message about error
    /// @return true if initialization was successed
    virtual bool OpenPackage(Package package_index, const char *filename, std::ostream &err) = 0;
};
