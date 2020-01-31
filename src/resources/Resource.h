#pragma once

#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>

namespace sci
{
/**
 * @defgroup SciApi SCI API
 * @brief The module provides a common API for working with resources.
 *
 * API can read fonts.
 * @{
 */

/**
 * @brief Shared API
 *
 * There is defined types, enumerations, constants that is used by resources.
 * Also interface provides the base API for working with files and resources.
 */
class Resource
{
public:
    /// @brief Type of resource.
    enum Type
    {
        view     = 0x80,    ///< View.
        pic      = 0x81,    ///< Background picture.
        script   = 0x82,    ///< Script.
        text     = 0x83,    ///< Text
        sound    = 0x84,    ///< Sound (MIDI music).
        object   = 0x85,    ///< Object is 'memory' resource, which is only used internally.
        vocab    = 0x86,    ///< Vocabulary.
        font     = 0x87,    ///< Font.
        cursor   = 0x88,    ///< Mouse cursor.
        patch    = 0x89,    ///< Audio patch file.
        bitmap   = 0x8a,    ///< Bitmap file.
        palette  = 0x8b,    ///< Pallete file.
        cd_audio = 0x8c,    ///< CD Audio resource.
        audio    = 0x8d,    ///< Audio resource (voice and digital sound effect).
        sync     = 0x8c,    ///< Sync
        message  = 0x8f,    ///< Message resource: Text plus metadata.
        map      = 0x90,    ///< Map location of resources for interpreter.
        dump     = 0x91,    ///< Heap resources: Dynamic script data.

        type_first = view,  ///< The first index of enumeration.
        type_last  = dump,  ///< The last index of enumeration.
        type_none  = 0xFF   ///< Index of undefined resource.
    };

    using Id = uint16_t;                ///< Type of index that is used by reading a resource.
    using Data = std::vector<uint8_t>;  ///< Type of container with resource's binary data.

    using Package = uint8_t;            ///< Type of index that is used for choose a package file with resources.
    using Position = uint32_t;          ///< Type of index that is used for choose a position of resource in a package file.

public:
    /// @brief Value of index that means nothing
    static constexpr auto s_id_none = std::numeric_limits<Id>::max();
    /// @brief Value of package that means nothing
    static constexpr auto s_package_none = std::numeric_limits<Package>::max();
    /// @brief Value of package that means nowhere
    static constexpr auto s_position_none = std::numeric_limits<Position>::max();

public:
    /**
     * @brief Initialization of a container with packages.
     * 
     * @param[in] package_amount
     *  Total number of packages.
     * @param[out] err
     *  A standard stream that will be got messages if the method has error.
     *
     * @return true if initialization is finished successfully.
     */
    virtual bool PreparePackages(Package package_amount, std::ostream &err) = 0;

    /**
     * @brief Initialization of a package with resources.
     *
     * @param[in] package_index
     *  Index of package.
     * @param[in] filename
     *  Name of file that contained resources.
     * @param[out] err
     *  A standard stream that will be got messages if the method has error.
     *
     * @return true if initialization is finished successfully.
     */
    virtual bool OpenPackage(Package package_index, const char *filename, std::ostream &err) = 0;

private:
    /**
     * @brief Default destructor
     *
     * The method is declared in private section, because the interface is used by inherited class only.
     */
    virtual ~Resource() = default;
};

///@} SciApi
} // sci
