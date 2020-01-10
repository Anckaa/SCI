#include <stdafx.h>
#include "ResourceImpl.h"

using namespace std;

bool ResourceImpl::Open(const char *filename, ostream &err)
{
    Close();
/*
    // TODO: need supporting for absolute path
    if(!m_map.Open("RESOURCE.MAP", err))
    {
        ERROR_TO_STREAM(err) << "map file doesn't open\n";
        return false;
    }
*/
    return true;
}

bool ResourceImpl::PreparePackages(Resource::Package amount, ostream &err)
{
/*
    m_packages.clear();

    try
    {
        m_packages.resize(amount);
    }
    catch (std::bad_alloc exception)
    {
        ERROR_TO_STREAM(err) << exception.what() << "\n";
        return false;
    }
*/
    return true;
}

bool ResourceImpl::OpenPackage(Resource::Package index, const char *filename, ostream &err)
{
/*
    if(m_packages.Empty())
    {
        ERROR_TO_STREAM(err) << "attempt to open unexsist package\n";
        return false;
    }

    if(index >= m_packages.size())
    {
        ERROR_TO_STREAM(err) << "index of package file is out of range\n";
        return false;
    }

    PackagePtr &package = m_packages[index];
    if(!package) package.reset(new cResourcePackage());
    return package->Open(filename, err);
*/
    return true;
}

void ResourceImpl::Close()
{
/*
    m_map.Close();
    m_packages.clear();
*/
}

const Resource::Data& ResourceImpl::Get(Type type, Id id, ostream &err) const
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

/*
bool ResourceImpl::Save(ostream &err)
{
    const cResourcePage::tResourcePtrArray& changing  = m_map.Changing();
    const cResourcePage::tResourcePtrArray& appending = m_map.Appending();

    if(changing.empty() && appending.empty())
        return true;

    cResourceMap::tOffsetArray offsets(0, { nullptr, s_package_index_none, s_resource_position_none, 0 });
    try
    {
        offsets.reserve(changing.size() + appending.size());
    }
    catch (std::bad_alloc exception)
    {
        err.assign(exception.what());
        return false;
    }

    std::for_each(changing.begin(), changing.end(), [&offsets](auto item)
    {
       offsets.push_back({item->data_ptr, item->package, item->position, 0});
    });
    std::sort(offsets.begin(), offsets.end(), [](const cResourceMap::cOffset &lft, const cResourceMap::cOffset &rgh) -> bool
    {
       return (lft.package == rgh.package) ? (lft.position < rgh.position) : (lft.package < rgh.package);
    });

    // TODO: prepare "temp" directory
    for(cResourceMap::tOffsetArray::iterator it_cur = offsets.begin(), it_end = offsets.end(); it_cur != it_end; )
    {
        if(it_cur->package >= m_packages.size())
        {
            err.assign("index of package is out of range");
            return false;
        }

        if(!CopyData(m_packages[it_cur->package], it_cur, it_end, err))
            return false;
    }

    return m_map.Save(offsets, err, false);
    return true;
}
*/
/*
bool ResourceImpl::CopyData(PackagePtr &package,
                            cResourceMap::tOffsetArray::iterator &offset_cur, const cResourceMap::tOffsetArray::iterator &offset_end, std::string &err)
{
    if(offset_cur == offset_end)
        return true;

    if(!package || !package->IsOpen())
    {
        err.assign(ErrorTag + "package isn't opening");
        return false;
    }

    std::string repack_path("./");
    repack_path.append(s_temp_dir);
    repack_path.append(package->Filename());

    cResourcePackage repack(std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!repack.Open(repack_path.c_str(), err))
        return false;

    tResourceId       data_id;
    eResourceType     data_type;
    eDecodeType       decode_type;
    tResourcePosition comp_size, real_size;

    package->Position(0);
    tPackageIndex package_cur = offset_cur->package;
    for(offset_cur; (offset_cur != offset_end) && (offset_cur->package == package_cur); ++offset_cur)
    {
        if(!package->CopyTo(offset_cur->position, repack, err))
            return false;

        if(!package->ReadHead(data_type, data_id, decode_type, comp_size, real_size, err))
            return false;

        if(!offset_cur->data_ptr)
        {
            offset_cur->offset = 0 - std::streamoff(comp_size + cResourcePackage::s_head_size);
        }
        else
        {
            // TODO: implement copy modify data
            return false;
        }

        package->Seek(comp_size);
    }

    return package->CopyTo(repack, err);
}
*/
