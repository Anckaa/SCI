#pragma once

#include <File.h>
#include "ResourcePage.h"

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
    ResourceMap(const ResourceMap &) = delete;
    ResourceMap(ResourceMap &&) = delete;
    ResourceMap& operator = (const ResourceMap &) = delete;
    ResourceMap&& operator = (const ResourceMap &&) = delete;

public:
    ResourceMap(Resource *owner) : File(std::ios_base::in | std::ios_base::binary), m_owner(owner) {}
    ResourceMap(std::ios_base::openmode mode) : File(mode), m_owner(nullptr) {}
    ~ResourceMap() { Reset(); }

    /// @overload File
    bool Open(const char *filename, std::ostream &err) override;
    void Close() override;

    const ResourcePage& At(Resource::Type type) const;
//    const ResourcePage& operator[](Resource::Type type) const { return m_pages[type]; }

/*
    bool Delete(eResourceType type, tResourceId id, std::ostream &err);
    bool Insert(eResourceType type, tResourceId id, const tResouceData &data, std::ostream &err);

    inline const ResourcePage::ResourcePtrArray& Changing()  { return m_changing;  }
    inline const ResourcePage::ResourcePtrArray& Appending() { return m_appending; }

    bool Save(tOffsetArray &offsets, std::ostream &err, bool sorting_offsets = true);
*/

protected:
    void Reset();

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

    Resource *m_owner;
    TypePages m_pages;
/*
    ResourcePage::ResourcePtrArray m_appending;
    ResourcePage::ResourcePtrArray m_changing;
*/
    HeadPosition ReadHead(Heads &head, std::ostream &err);
    bool ReadPage(HeadPosition from, HeadPosition to, Packages &packages, ResourcePage &resources, std::ostream &err);
};
