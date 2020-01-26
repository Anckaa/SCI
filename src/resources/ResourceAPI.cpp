#include <stdafx.h>
#include "ResourceAPI.h"

using namespace std;

bool ResourceAPI::Open(const char *filename, ostream &err)
{
    Close();

    // TODO: need supporting for absolute path
    if(!m_map.Open(filename, err))
    {
        ERROR_TO_STREAM(err) << "map file doesn't open\n";
        return false;
    }

    return true;
}

bool ResourceAPI::PreparePackages(Package package_amount, ostream &err)
{
    m_packages.clear();

    try
    {
        m_packages.resize(package_amount);
    }
    catch (std::bad_alloc exception)
    {
        ERROR_TO_STREAM(err) << exception.what() << "\n";
        return false;
    }

    return true;
}

bool ResourceAPI::OpenPackage(Resource::Package package_index, const char *filename, ostream &err)
{
    if(m_packages.empty())
    {
        ERROR_TO_STREAM(err) << "attempt to open unexsist package\n";
        return false;
    }

    if(package_index >= m_packages.size())
    {
        ERROR_TO_STREAM(err) << "index of package file is out of range\n";
        return false;
    }

    auto &package = m_packages[package_index];
    if (!package)
    {
        package.reset(new cResourcePackage());
    }
    return package->Open(filename, err);
}

void ResourceAPI::Close()
{
    m_map.Close();
    m_packages.clear();
}

const Resource::Data& ResourceAPI::Get(Type type, Id id, ostream &err) const
{
    static const Data data_empty{0};
/*
    const cResourcePage::cResourceItem &item = m_map[type][id];
    if(!item.IsAllocated())
    {
        err.assign(ErrorTag + "index of resource item is incorrect");
        return data_empty;
    }

    if(item.data_ptr != nullptr)
        return *item.data_ptr;

    if(item.package > m_packages.size())
    {
        err.assign(ErrorTag + "index of package file is out of range");
        return data_empty;
    }

    PackagePtr &package = m_packages[item.package];
    if(!package)
    {
        err.assign(ErrorTag + "resource file is not opened");
        return data_empty;
    }

    // TODO: implement allocator for placing new buffer; it should blocking segmantation of memory
    if(!package->ReadData(type, id, item.position, m_buffer, err))
    {
        MarkError(err);
        return data_empty;
    }

// TODO: ???    desc.data_ptr = &m_buffer;
    return m_buffer;
*/
    return data_empty;
}
