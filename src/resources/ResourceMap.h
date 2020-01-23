#pragma once

#include <File.h>
#include "ResourcePage.h"

/// @brief Resouce's map accessor
class ResourceMap : protected File
{
public:
    struct Item
    {
        const Resource::Data *data_ptr;
        Resource::Package     package;
        Resource::Position    position;
        Resource::Offset      offset;
    };
    using Items = std::vector<Item>;


public:
    /// @brief Constructors and destructor
    ResourceMap() : File(std::ios_base::in | std::ios_base::binary) {}
    ResourceMap(const ResourceMap &) = delete;
    ResourceMap(ResourceMap &&) = delete;
    ResourceMap& operator = (const ResourceMap &) = delete;
    ResourceMap&& operator = (const ResourceMap &&) = delete;
    ~ResourceMap() { Clear(); }

    /// @overload File
    /// @{
    bool Open(const char *filename, std::ostream &err) override;
    void Close() override;
    /// @}

    /// @brief Get page with resources for specified type
    /// @param type - type of resources (see to Resource::Type)
    /// @return const reference to the page
    const ResourcePage& operator[](Resource::Type type) const { return m_pages[type]; }

protected:
    /// @brief Clear all map data
    void Clear();

private:
    using HeadPosition = uint16_t;
    struct HeadData
    {
        Resource::Type type;
        HeadPosition position;
    };

    using Heads = std::list<HeadData>;
    using Packages = std::vector<bool>;
    using TypePages = std::map<Resource::Type, ResourcePage>;

private:
    static const std::size_t s_head_size = 3;           // head of resource's map (in bits): 8 (type) 16 (position)
    static const std::size_t s_page_item_size = 6;      // page item of resource (in bits): 16 (id) + 28 (position) + 4 (index of package)
    static const Resource::Package s_package_size = 16; // total packages involved (index for filename "RESOURCE.XXX")

    TypePages m_pages;

    /// @brief Reading fields of heads
    /// @param head - reference to list of heads
    /// @param err  - standard stream for output message about error
    /// @return The first position in file after head
    Resource::Position ReadHead(Heads &head, std::ostream &err);

    /// @brief Reading fields of resources
    /// @param from     - position in file for begin reading
    /// @param to       - position in file of end reading
    /// @param packages - list with using packages
    /// @param resources- list of pages with resources
    /// @param err      - standard stream for output message about error
    /// @return true if reading is finished sucessfully
    bool ReadPage(Resource::Position from, Resource::Position to, Packages &packages, ResourcePage &resources, std::ostream &err);
};
