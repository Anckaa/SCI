#pragma once

#include <File.h>
#include "ResourcePage.h"

namespace sci
{
///@addtogroup SciApi
///@{

/**
 * @brief Resouce's map accessor
 *
 * A class provides information about location of resource.
 */
class ResourceMap : protected File
{
public:
    /**
     * @brief The Item struct
     *
     * A data of location to resource.
     */
    struct Item
    {
        const Resource::Data *data_ptr; ///< A pointer to binary data of resource.
        Resource::Package package;      ///< An index of package that contains the resource.
        Resource::Position position;    ///< A position of resource in the package.
    };

    using Items = std::vector<Item>;    ///< Collection of the locations.

public:
    /// @brief Default Constructor.
    ResourceMap();

    /// @brief Default Destructor.
    ~ResourceMap();

    ResourceMap(const ResourceMap &) = delete;                  ///< @private using a method is denied.
    ResourceMap(ResourceMap &&) = delete;                       ///< @private using a method is denied.
    ResourceMap& operator = (const ResourceMap &) = delete;     ///< @private using a method is denied.
    ResourceMap&& operator = (const ResourceMap &&) = delete;   ///< @private using a method is denied.


    /// @overload File
    /// @{
    bool Open(const char *filename, std::ostream &err) noexcept override;
    void Close() noexcept override;
    /// @}

    /**
     * @brief Get page with resource locations for specified type.
     *
     * @param[in] type
     *  Type of resource.
     *
     * @return reference to the page.
     */
    const ResourcePage& operator[](Resource::Type type) const;

private:
    TypePages m_pages;  ///< A collection of the pages.

private:
    /// @brief Clear all data.
    void Clear() noexcept;
};

///@} SciApi
} // sci
