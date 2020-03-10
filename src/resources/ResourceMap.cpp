#include <stdafx.h>
#include "ResourceMap.h"

namespace sci
{

namespace
{

using HeadPosition = uint16_t;  ///< A type of head position.

/**
 * @brief The HeadData struct.
 *
 * A description of resource head.
 */
struct HeadData
{
    Resource::Type type;    ///< A type of resource.
    HeadPosition position;  ///< A position of page with detail information about resource.
};

using Heads = std::list<HeadData>;  ///< A collection of heads
using Packages = std::vector<bool>; ///< A collection with flag whether a package with according index is used.

static const std::size_t s_head_size = 3;           ///< A size of head (in bytes): 8 bits (type) + 16 bits (position).
static const std::size_t s_page_item_size = 6;      ///< A size of page item (in bytes):
                                                    ///<    16 bits (id) + 28 bits (position in package) + 4 bits (index of package).
static const Resource::Package s_package_size = 16; ///< Total packages involved (index for filename "RESOURCE.XXX").

/**
 * @brief Reading fields of heads.
 *
 * @param[in,out] file
 *  Reference to file for reading.
 * @param[out] head
 *  Reference to list of heads.
 * @param[out] err
 *  A standard stream that will be got messages if the method has error.
 *
 * @return The first position in file after reading all heads.
 */
Resource::Position ReadHead(File &file, Heads &head, std::ostream &err)
{
/*
    tHeadPosition position_total = 0;

    File::Position(0);
    std::streampos previous_position = 0, current_position = 0;
    for(uint8_t type = cFile::Get(); !cFile::IsEof(); type = cFile::Get())
    {
        if(type == s_resource_type_null)
        {
            head.push_back({ static_cast<eResourceType>(type), cFile::Get16LE() });

            position_total = tHeadPosition(cFile::Position());
            if(s_head_size != position_total - previous_position)
            {
                err.assign("file size is incorrectly");
                return false;
            }

            break;
        }

        if((type < s_resource_type_first) || (type > s_resource_type_last))
        {
            err.assign("recource type is incorrectly");
            return false;
        }

        head.push_back({ static_cast<eResourceType>(type), Get16LE() });

        current_position = Position();
        if(s_head_size != current_position - previous_position)
        {
            err.assign("file size is incorrectly");
            return false;
        }

        previous_position = current_position;
    }

    if(!position_total)
        err.assign("map file has no data");
    return position_total;
*/
    return 0;
}

/**
 * @brief Reading fields of detail resource information.
 *
 * @param[in,out] file
 *  Reference to file for reading.
 * @param[in] from
 *  A position in file for begin reading.
 * @param[in] to
 *  A position in file of end reading.
 * @paramp[in,out] packages
 *  A list with using packages.
 * @param[in,out] resources
 *  Pages with resource descriptions.
 * @param[out] err
 *  A standard stream that will be got messages if the method has error.
 *
 * @return true if reading is finished sucessfully.
 */
bool ReadPage(File &file, Resource::Position from, Resource::Position to, Packages &packages, ResourcePage &resources, std::ostream &err)
{
/*
    if(left >= right)
        return false;

    std::streampos current_position = left;
    cFile::Position(current_position);
    if(cFile::Position() != current_position)
    {
        err.assign("file has incorrect size");
        return false;
    }

    tResourceId       id = 0;
    tPackageIndex     package_index = 0;
    tResourcePosition position_lo = 0, position_hi = 0;

    std::streampos previous_position = current_position;
    do
    {
        id          = cFile::Get16LE();
        position_lo = cFile::Get24LE();
        position_hi = cFile::Get();

        current_position = cFile::Position();
        if(s_page_item_size != current_position - previous_position)
        {
            err.assign("file size is incorrectly");
            return false;
        }

        package_index = position_hi & 0x0F;
        if(package_index >= s_package_size)
        {
            err.assign("index of package is incorrectly");
            return false;
        }

        packages[package_index] = true;
        resources.Allocate(package_index, position_lo | ((position_hi & 0xF0) << 20), id, err);
        previous_position = current_position;
    } while(current_position < right);
*/
    return true;
}

}   // unnamed

ResourceMap::ResourceMap()
    : File(std::ios_base::in | std::ios_base::binary)
{
}

ResourceMap::~ResourceMap()
{
    Clear();
}

bool ResourceMap::Open(const char *filename, std::ostream &err) noexcept
{
/*
    Close();
    if(!cFile::Open(filename, err))
        return false;

    if(!m_resource || (cFile::Mode() & std::ios_base::out))
        return true;

    tHeadArray head;
    tHeadPosition head_size = ReadHead(head, err);
    if(!head_size)
        return false;

    head.sort([](const cHeadData &lft, const cHeadData &rgh) -> bool
    {
        return lft.position < rgh.position;
    });

    if(head.front().position < head_size)
    {
        err.assign("map file is incorrectly");
        return false;
    }

    cResourcePage   resources;
    tPackageIndexes package_indexes;
    try
    {
        package_indexes.resize(s_package_size, false);
    }
    catch (std::bad_alloc exception)
    {
        err.assign(exception.what());
        return false;
    }

    for(tHeadArray::const_iterator end_it = head.cend(), rgh_it = head.cbegin(), lft_it = rgh_it++; rgh_it != end_it; lft_it = rgh_it++)
    {
        if(!ReadPage(lft_it->position, rgh_it->position, package_indexes, resources, err))
            return(false);

        m_resource_map[lft_it->type] = std::move(resources);
    }

    tPackageIndex package_cur = s_package_size;
    while(true)
    {
        if(package_indexes[--package_cur])
            break;
        if(!package_cur)
            return true;
    }

    char package_name[256];
    if(!m_resource->PreparePackages(++package_cur, err))
        return false;

    for(package_cur; (package_cur--) > 0; )
    {
        if(package_indexes[package_cur])
        {

            sprintf(package_name, "RESOURCE.%03u", package_cur);
            if(!m_resource->OpenPackage(package_cur, package_name, err))
                return false;
        }
    }
*/
    return true;
}

const ResourcePage& ResourceMap::operator[](Resource::Type type) const
{
    return m_pages[type];
}

void ResourceMap::Clear() noexcept
{
    m_pages.clear();
}

void ResourceMap::Close() noexcept
{
    Clear();
    File::Close();
}

/*
bool ResourceMap::Delete(eResourceType type, tResourceId id, std::string &err)
{
    if((type < eResourceType::first) || (type > eResourceType::last))
    {
        err.assign("resource for specific type is out of range");
        return false;
    }

    tResourceMap::iterator page = m_resource_map.find(type);
    if(page == m_resource_map.end())
    {
        err.assign("resource for specific type isn't search");
        return false;
    }

    cResourcePage::tResourcePtr item = nullptr;
    if(!page->second.Delete(id, item, err))
        return false;

    if(!item)
    {
        err.assign("description of resource isn't exist");
        return false;
    }

    if(item->IsAllocated())
        m_changing.insert(item);
    else
        m_appending.erase(item);
    return true;
}

bool ResourceMap::Insert(eResourceType type, tResourceId id, const tResouceData &data, std::string &err)
{
    if((type < eResourceType::first) || (type > eResourceType::last))
    {
        err.assign("resource for specific type is out of range");
        return false;
    }

    tResourceMap::iterator page = m_resource_map.find(type);
    if(page == m_resource_map.end())
    {
        err.assign("resource for specific type isn't search");
        return false;
    }

    cResourcePage::tResourcePtr item = nullptr;
    if(!page->second.Insert(id, data, item, err))
        return false;

    if(!item)
    {
        err.assign("description of resource isn't exist");
        return false;
    }

    if(item->IsAllocated())
        m_changing.insert(item);
    else
        m_appending.insert(item);

    return true;
}

bool ResourceMap::Save(tOffsetArray &offsets, std::ostream &err, bool sorting_offsets/* = true/)
{
    if(sorting_offsets)
        std::sort(offsets.begin(), offsets.end(), [](const cResourceMap::cOffset &lft, const cResourceMap::cOffset &rgh) -> bool
        {
           return (lft.package == rgh.package) ? (lft.position < rgh.position) : (lft.package < rgh.package);
        });

    std::string destination_path("./");
    destination_path.append(IResource::s_output_dir);
    destination_path.append(CFile::Filename());

    cResourceMap destination(std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!destination.Open(destination_path.c_str(), err))
        return false;
    return true;
}
*/

} // sci
