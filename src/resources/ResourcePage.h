#pragma once

#include "Resource.h"

class ResourcePage
{
//    friend class ResourceMap;

public:
    struct Item
    {
        Resource::Package package;
        Resource::Position position;
        const Resource::Data *data_ptr;

        inline bool Check() const       { return ((package == Resource::s_package_none) == (position == Resource::s_position_none)); }
        inline bool IsAllocated() const { return ((package != Resource::s_package_none) && (position != Resource::s_position_none)); }
        inline bool IsEmpty() const     { return ((data_ptr == nullptr) || data_ptr->empty());                                       }
    };

public:
    ResourcePage(ResourcePage &&) = delete;
    ResourcePage(const ResourcePage &) = delete;
    ResourcePage& operator = (const ResourcePage &) = delete;

public:
    ResourcePage() : m_items(s_page_size, s_item_empty) {}
    ResourcePage(std::size_t size) : m_items(size, s_item_empty) {}
    ResourcePage& operator = (ResourcePage &&);
/*
    Resource::Id Id(const Item *item) const;
    inline Resource::Id Id(const Item &item) const  { return Id(&item);        }
    const Item& operator[](Resource::Id id) const   { return m_resources[id];  }
*/
    inline bool IsEmpty() const                     { return m_using.empty();  }
    inline std::size_t Amount() const               { return m_using.size();   }
/*
    ItemsPtr::const_iterator begin() const { return m_using.cbegin(); }
    ItemsPtr::const_iterator end() const   { return m_using.cend();   }
*/

protected:
    inline void Clear() { m_using.clear(); }

    bool Allocate(Resource::Package package, Resource::Position position, Resource::Id id, std::ostream &err);

private:
    using Items = std::vector<Item>;
    using ItemsPtr = std::set<Item *>;

private:
    static constexpr std::size_t s_page_size = 1000;
    static constexpr Item s_item_empty{};

    Items    m_items;
    ItemsPtr m_using;
};
