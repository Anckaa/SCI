#include <stdafx.h>
#include "ResourcePage.h"

using namespace std;

ResourcePage& ResourcePage::operator = (ResourcePage &&that)
{
    m_items.swap(that.m_items);
    m_using.swap(that.m_using);
    return *this;
}

bool ResourcePage::Allocate(Resource::Package package, Resource::Position position, Resource::Id id, ostream &err)
{
/*
    if(id >= m_resources.size())
    {
        err.assign("index is out of range");
        return false;
    }

    if((package == s_package_index_none) || (position == s_resource_position_none))
    {
        err.assign("allocation is incorrectly");
        return false;
    }

    cResourceItem &item = m_resources[id];
    if(item.IsAllocated())
    {
        err.assign("resource allocated already");
        return false;
    }

    if(!item.IsEmpty())
    {
        err.assign("FATAL ERROR: resource has data but it doesn't allocate");
        return false;
    }

    item.package  = package;
    item.position = position;
    m_using.insert(&item);
*/
    return true;
}

/*
bool ResourcePage::Insert(tResourceId id, const tResouceData &data, tResourcePtr &item, ostream &err)
{
    if(data.empty())
    {
        err.assign("attempt to insert empty to resource");
        item = nullptr;
        return false;
    }

    if(id >= m_resources.size())
    {
        err.assign("resource's index is out of range");
        item = nullptr;
        return false;
    }

    item = &m_resources[id];
    // TODO: remove data from memory before updating
    item->data_ptr = reinterpret_cast<const tResouceData *>(data.data());
    m_using.insert(item);
    return true;
}

bool ResourcePage::Delete(tResourceId id, tResourcePtr &item, ostream &err)
{
    if(id >= m_resources.size())
    {
        err.assign("index is out of range");
        item = nullptr;
        return false;
    }

    item = &m_resources[id];
    // TODO: remove data from memory
    item->data_ptr = nullptr;
    m_using.erase(item);

    return true;
}
*/

/*
Resource::Id ResourcePage::Id(const ResourcePage::Item *item) const
{
//    if(!item)
        return s_resource_id_none;

    const cResourceItem *front = m_resources.data();
    if((item < front) || (item > &m_resources.back()))
        return s_resource_id_none;
    return tResourceId(item - front);
}
*/
