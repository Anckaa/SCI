#pragma once

class Resource
{
public:
    enum Type
    {
        view     = 0x80,
        pic      = 0x81,
        script   = 0x82,
        text     = 0x83,
        sound    = 0x84,
        object   = 0x85,
        vocab    = 0x86,
        font     = 0x87,
        cursor   = 0x88,
        patch    = 0x89,
        bitmap   = 0x8a,
        palette  = 0x8b,
        cd_audio = 0x8c,
        audio    = 0x8d,
        sync     = 0x8c,
        message  = 0x8f,
        map      = 0x90,
        dump     = 0x91,

        first = view,
        last  = dump,
        none  = 0xFF
    };

    using Id = uint16_t;
    using Data = std::vector<uint8_t>;

    using Package = uint8_t;
    using Position = uint32_t;
    using Offset = int64_t;

public:
    static const Id s_id_none = -1;
    static const Package s_package_none = -1;
    static const Position s_position_none = -1;

public:
    virtual ~Resource() {}

    virtual bool PreparePackages(Package amount, std::ostream &err) = 0;
    virtual bool OpenPackage(Package index, const char *filename, std::ostream &err) = 0;
};
