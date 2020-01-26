#pragma once

#include "ResourceMap.h"
#include "ResourcePackage.h"

/**
 * @brief Resource accessor
 *
 * A class provides a base management with resources: to open files, to get resource, to replace it and etc.
 */
class ResourceAPI : private Resource
{
public:
    /// @brief Default constructor.
    ResourceAPI() : m_map(this) {}
    ResourceAPI(ResourceAPI &&) = delete;                       /// < Using of method is denied
    ResourceAPI(const ResourceAPI &) = delete;                  /// < Using of method is denied
    ResourceAPI& operator = (const ResourceAPI &) = delete;     /// < Using of method is denied
    ResourceAPI&& operator = (const ResourceAPI &&) = delete;   /// < Using of method is denied

    /**
     * @brief Reads resources.
     *
     * @param[in] filename
     *  Name of map file with every description of resource.
     * @param[in,out] err
     *  Standard io stream for output an error.
     *
     * @return true if all resources were opened successfully.
     */
    bool Open(const char *filename, std::ostream &err);

    /// @brief Clears all resourcess and closes all opened files.
    void Close();

    /**
     * @brief Gets page with description of all resources for specified type.
     *
     * @param[in] type
     *  Type of resource.
     *
     * @return Reference to the page.
     */
    const ResourcePage& operator[](Type type) const { return m_map[type]; }

    /**
     * @brief Gets a source data of resource.
     *
     * @param[in] type
     *  Type of resource.
     * @param[in] Id
     *  Index of resource.
     * @param[in,out] err
     *  Standard io stream for output an error.
     *
     * @return Reference to the source data of resource.
     */
    const Data& Get(Type type, Id id, std::ostream &err) const;

private:
    using Packages = std::vector<ResourcePackage*>;

private:
    ResourceMap m_map;
    Packages    m_packages;

    /// @implements Resource
    /// @{
    bool PreparePackages(Package package_amount, std::ostream &err) override;
    bool OpenPackage(Package package_index, const char *filename, std::ostream &err) override;
    /// @}

    /// @brief Clear all resources.
    void Reset();
};
