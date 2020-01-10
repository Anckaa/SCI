#include <stdafx.h>
#include "cResourcePackage.h"

bool cResourcePackage::Open(const char *filename, std::string &err)
{
    Close();
    return cFile::Open(filename, err);
}

bool cResourcePackage::ReadHead(eResourceType &type, tResourceId &id,
                                eDecodeType &decode, tResourcePosition &comp_size, tResourcePosition &real_size, std::string &err)
{
    std::streampos position = cFile::Position();
    tResourceType  type_id  = cFile::Get();
    if((type_id < s_resource_type_first) || (type_id > s_resource_type_last))
    {
        err.assign("reading type of resource is incorrectly");
        return false;
    }

    type      = static_cast<eResourceType>(type_id);
    id        = cFile::Get16LE();
    comp_size = cFile::Get16LE();
    real_size = cFile::Get16LE();
    tDecodeType decode_id = tDecodeType(cFile::Get16LE());
    if((decode_id != s_decode_type_none) && ((decode_id < s_decode_type_first) || (decode_id > s_decode_type_last)))
    {
        err.assign("reading decode id for resource is incorrectly");
        return false;
    }

    decode = static_cast<eDecodeType>(decode_id);
    if(s_head_size != cFile::Position() - position)
    {
        err.assign("resource file size is incorrectly");
        return false;
    }

    return true;
}

bool cResourcePackage::ReadData(eResourceType type, tResourceId id, tResourcePosition position, tResouceData &data, std::string &err)
{
    if(position == s_resource_position_none)
    {
        err.assign("file position of resource is none");
        return false;
    }

    cFile::Position(position);
    if(cFile::Position() != std::streampos(position))
    {
        err.assign("file position of resource is out of range");
        return false;
    }

    eResourceType     data_type;
    tResourceId       data_id;
    eDecodeType       decode_type;
    tResourcePosition comp_size, real_size;
    if(!ReadHead(data_type, data_id, decode_type, comp_size, real_size, err))
        return false;

    if((type != data_type) || (id != data_id))
    {
        err.assign("resource has other data in setting position");
        return false;
    }

    if(decode_type == eDecodeType::none)
    {
        if(comp_size != real_size)
        {
            err.assign("resource data has difference in size between compressing and uncompressing");
            return false;
        }
        return cFile::Read(data, comp_size, err);
    }

    cDecode *decoder = Decoder(decode_type, err);
    if(!decoder)
        return false;

    if(!cFile::Read(m_buffer, comp_size, err))
        return false;

    try
    {
        data.resize(real_size);
    }
    catch (std::bad_alloc exception)
    {
        err.assign(exception.what());
        return false;
    }

    return (decoder->Apply(m_buffer.data(), m_buffer.size(), data.data(), data.size(), err) == real_size);
}

cDecode* cResourcePackage::Decoder(eDecodeType type, std::string &err)
{
    tDecoders::iterator decoder_it = m_decoders.find(type);
    if(decoder_it != m_decoders.end())
        return decoder_it->second.get();

    cDecode *decoder = cDecode::Make(type);
    if(!decoder)
    {
        err.assign("decode isn't support (or out of memory)");
        return nullptr;
    }

    m_decoders[type] = tDecodeUPtr(decoder);
    return decoder;
}
