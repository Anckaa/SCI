#pragma once

#include "ResourceMap.h"
#include "ResourcePackage.h"

/// @brief Resource accessor
class ResourceAPI : private Resource
{
public:
    /// @brief Constructors
    ResourceAPI() : m_map(this) {}
    ResourceAPI(ResourceAPI &&) = delete;
    ResourceAPI(const ResourceAPI &) = delete;
    ResourceAPI& operator = (const ResourceAPI &) = delete;
    ResourceAPI&& operator = (const ResourceAPI &&) = delete;

    /// @brief Open map file with resources
    /// @param filename - name of map file
    /// @param err      - standard stream for output message about error
    /// @return true if all resources were opened successfully
    bool Open(const char *filename, std::ostream &err);

    /// @brief Close the current map file and clear all resourcess
    void Close();

    /// @brief Get page with resources for specified type
    /// @param type - type of resources (see to Resource::Type)
    /// @return const reference to the page
    const ResourcePage& operator[](Type type) const { return m_map[type]; }

    /// @brief Get binary data of resource
    /// @param type - type of resource (see to Resource::Type)
    /// @param Id   - Index of resource
    /// @param err  - standard stream for output message about error
    /// @return reference to the binary data
    const Data& Get(Type type, Id id, std::ostream &err) const;
/*
    inline bool Delete(ResourceType type, Id id, std::ostream &err) { return m_map.Delete(type, id, err); }
    inline bool Insert(ResourceType type, Id id, const BinaryData &data, std::ostream &err) { return m_map.Insert(type, id, data, err); }
*/
//    bool Save(std::ostream &err);

private:
    using Packages = std::vector<ResourcePackage*>;

private:
    ResourceMap m_map;
    Packages    m_packages;
// TODO: ???    Data        m_buffer;

/* TODO: ???
    bool CopyData(PackagePtr &package,
                  ResourceMap::tOffsetArray::iterator &offset_cur, const cResourceMap::tOffsetArray::iterator &offset_end, std::ostream &err);
*/
    /// @implements Resource
    /// @{
    bool PreparePackages(Package package_amount, std::ostream &err) override;
    bool OpenPackage(Package package_index, const char *filename, std::ostream &err) override;
    /// @}

    /// @brief Clear all resources
    void Reset();
};
