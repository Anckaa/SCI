#include <stdafx.h>
#include "ResourcePackage.h"

using namespace std;

bool ResourcePackage::ReadHead(Resource::Type &type, Resource::Id &id,
                               Decode::Type &decode, Resource::Position &pack_size, Resource::Position &real_size, std::ostream &err) noexcept
{
    try
    {
        const auto position     = File::Position();
        const auto type_value   = Get();
        id                      = Get16LE();
        pack_size               = Get16LE();
        real_size               = Get16LE();
        const auto decode_value = Get16LE();
        static const Resource::Position s_head_size = 9;    // summ of reading bytes is 9

        if(File::Position() - position != s_head_size)
        {
            throw ios_base::failure("resource file size is incorrectly"_s);
        }

        if(type_value < Resource::type_first || type_value > Resource::type_last)
        {
            throw ios_base::failure("reading type of resource is incorrectly"_s);
        }
        type = static_cast<Resource::Type>(type_value);

        if(decode_value != Decode::type_none && (decode_value < Decode::type_first || decode_value > Decode::type_last))
        {
            throw ios_base::failure("reading decode id for resource is incorrectly"_s);
        }
        decode = static_cast<Decode::Type>(decode_value);
    }
    catch (const ios_base::failure& ex)
    {
        ERROR_TO_STREAM(err) << ex.what() << "\n";
        return false;
    }

    return true;
}

bool ResourcePackage::ReadData(Resource::Type type, Resource::Id id, Resource::Position position, Resource::Data &data, std::ostream &err) noexcept
{
    try
    {
        File::Position(position);
        if(File::Position() != static_cast<std::streampos>(position))
        {
            throw ios_base::failure("file position of resource is out of range"_s);
        }
    }
    catch (const std::ios_base::failure& ex)
    {
        ERROR_TO_STREAM(err) << ex.what() << "\n";
        return false;
    }

    Resource::Type     head_type;
    Resource::Id       head_id;
    Decode::Type       head_decode;
    Resource::Position head_pack_size, head_real_size;
    if(!ReadHead(head_type, head_id, head_decode, head_pack_size, head_real_size, err))
    {
        ERROR_TO_STREAM(err) << "reading head is failed\n";
        return false;
    }

    if((type != head_type) || (id != head_id))
    {
        ERROR_TO_STREAM(err) << "resource has other data in setting position\n";
        return false;
    }

    if(head_decode == Decode::type_none)
    {
        if(head_pack_size != head_real_size)
        {
            ERROR_TO_STREAM(err) << "resource data has difference in size between compressing and uncompressing\n";
            return false;
        }

        return File::Read(data, head_pack_size, err);
    }

    Decode *decoder = Decode::Make(head_decode);
    if(!decoder)
    {
        return false;
    }

    Data pack_data;
    if(!File::Read(pack_data, head_pack_size, err))
    {
        return false;
    }

    try
    {
        data.resize(head_real_size);
    }
    catch (std::bad_alloc ex)
    {
        ERROR_TO_STREAM(err) << ex.what() << "\n";
        return false;
    }

    return (decoder->Apply(pack_data.data(), head_pack_size, data.data(), head_real_size, err) == head_real_size);
}
