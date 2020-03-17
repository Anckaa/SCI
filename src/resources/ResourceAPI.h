/**
 * @file ResourceAPI.h
 * @brief This file defines a base management with SCI resources.
 * @author Anatoly Tereschenko
 */

#pragma once

#include "ResourceMap.h"
#include "ResourcePackage.h"

namespace sci
{
namespace api
{

///@addtogroup SciApi
///@{

/**
 * @brief Resource accessor
 *
 * A class provides a base management with resources: to open files, to get resource, to replace it and etc.
 */
class ResourceAPI : private Resource
{
public:
    /// @brief Default constructor.
    ResourceAPI();

    ResourceAPI(ResourceAPI &&) = delete;                       ///< @private using a method is denied
    ResourceAPI(const ResourceAPI &) = delete;                  ///< @private using a method is denied
    ResourceAPI& operator = (const ResourceAPI &) = delete;     ///< @private using a method is denied
    ResourceAPI&& operator = (const ResourceAPI &&) = delete;   ///< @private using a method is denied

    /**
     * @brief Preparing files with resources.
     *
     * @param[in] filename
     *  Name of map file with all descriptions.
     * @param[out] err
     *  A standard stream that will be got messages if the method has error.
     *
     * @return true if all resources is opened successfully.
     */
    bool Open(const char *filename, std::ostream &err) noexcept;

    /// @brief All opened resources will been freed, and all opened files will been closed.
    void Close();

    /**
     * @brief Getting a page of descriptions for all resources with specified type.
     *
     * @param[in] type
     *  Type of resource.
     *
     * @return Reference to the page.
     */
    const ResourcePage& operator[](Type type) const;

    /**
     * @brief Getting a source data of resource.
     *
     * @param[in] type
     *  Type of resource.
     * @param[in] id
     *  Index of resource.
     * @param[out] err
     *  A standard stream that will be got messages if the method has error.
     *
     * @return Reference to the source data of resource.
     */
    const Data& Get(Type type, Id id, std::ostream &err) const;

private:
    using Packages = std::vector<ResourcePackage*>; ///< A type for container of packages with resources.

private:
    ResourceMap m_map;      ///< Map with all descriptions of resources.
    Packages m_packages;    ///< A collection of packages with resources.

    /// @implements Resource
    /// @{
    bool PreparePackages(Package package_amount, std::ostream &err) noexcept override;
    bool OpenPackage(Package package_index, const char *filename, std::ostream &err) noexcept override;
    /// @}

    /// @brief Clear all resources.
    void Reset() noexcept;
};

///@} SciApi
} // api
} // sci
