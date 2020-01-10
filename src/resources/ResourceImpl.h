#pragma once

#include "ResourceMap.h"
//#include "ResourcePackage.h"

class ResourceImpl : protected Resource
{
public:
    ResourceImpl() : m_map(this) {}

    ResourceImpl(const ResourceImpl &) = delete;
    ResourceImpl(ResourceImpl &&) = delete;
    ResourceImpl& operator = (const ResourceImpl &) = delete;
    ResourceImpl&& operator = (const ResourceImpl &&) = delete;

    bool Open(const char *filename, std::ostream &err);
    void Close();

    inline const ResourcePage& At(Type type) const         { return m_map.At(type); }
//    inline const ResourcePage& operator[](Type type) const { return m_map[type];    }

    const Data& Get(Type type, Id id, std::ostream &err) const;
/*
    inline bool Delete(ResourceType type, Id id, std::ostream &err) { return m_map.Delete(type, id, err); }
    inline bool Insert(ResourceType type, Id id, const BinaryData &data, std::ostream &err) { return m_map.Insert(type, id, data, err); }
*/
//    bool Save(std::ostream &err);

private:
/*
    using PackagePtr = std::unique_ptr<ResourcePackage>;
    using Packages   = std::vector<PackagePtr>;
*/

private:
    ResourceMap m_map;
//    Packages    m_packages;
    Data m_buffer; // TODO: ???

/*
    bool CopyData(PackagePtr &package,
                  ResourceMap::tOffsetArray::iterator &offset_cur, const cResourceMap::tOffsetArray::iterator &offset_end, std::ostream &err);
*/
    /// @implement Resource
    bool PreparePackages(Package amount, std::ostream &err) override;
    bool OpenPackage(Package index, const char *filename, std::ostream &err) override;

    void Reset();
};
