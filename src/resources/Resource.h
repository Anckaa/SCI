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
        view     = 0x80,    ///< View
        pic      = 0x81,    ///< Picture
        script   = 0x82,    ///< Script
        text     = 0x83,    ///< Text
        sound    = 0x84,    ///< Sound
        object   = 0x85,    ///< Object
        vocab    = 0x86,    ///< Vocabulary
        font     = 0x87,    ///< Font
        cursor   = 0x88,    ///< Cursor
        patch    = 0x89,    ///< Patch
        bitmap   = 0x8a,    ///< Bitmap
        palette  = 0x8b,    ///< Pallete
        cd_audio = 0x8c,    ///< CD Audio
        audio    = 0x8d,    ///< Audio
        sync     = 0x8c,    ///< Sync
        message  = 0x8f,    ///< Message
        map      = 0x90,    ///< Map
        dump     = 0x91,    ///< Dump

        type_first = view,  ///< The first index of enumeration.
        type_last  = dump,  ///< The last index of enumeration.
        type_none  = 0xFF   ///< Index of undefined resource.
    };

    using Id = uint16_t;
    using Data = std::vector<uint8_t>;

    using Package = uint8_t;
    using Position = uint32_t;

/* TODO: !!! Look for access to type
private:
    using Offset = int64_t;
*/
public:
    static constexpr auto s_id_none = std::numeric_limits<Id>::max();
    static constexpr auto s_package_none = std::numeric_limits<Package>::max();
    static constexpr auto s_position_none = std::numeric_limits<Position>::max();

public:
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

private:
    virtual ~Resource() = default;
};
