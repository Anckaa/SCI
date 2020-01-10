#pragma once

#include <File.h>
#include <cDecode.h>

class cResourcePackage : protected cFile
{
public:
    static const tResourcePosition s_head_size = 9;

public:
    cResourcePackage(const cResourcePackage &) = delete;
    cResourcePackage(cResourcePackage &&) = delete;
    cResourcePackage& operator = (const cResourcePackage &) = delete;
    cResourcePackage&& operator = (const cResourcePackage &&) = delete;

public:
    cResourcePackage() : cFile(std::ios_base::in | std::ios_base::binary) {}
    cResourcePackage(std::ios_base::openmode mode) : cFile(mode) {}

    virtual bool Open(const char *filename, std::string &err) override;

    inline bool IsOpen()                     { return cFile::IsOpen(); }
    inline const char* Filename()            { return cFile::Filename(); }
    inline std::streampos Position()         { return cFile::Position(); }
    inline void Position(std::streampos pos) { cFile::Position(pos);     }
    inline void Seek(std::streamoff offset)  { cFile::Seek(offset);      }
    inline bool IsEof()                      { return cFile::IsEof();    }

    bool ReadHead(eResourceType &type, tResourceId &id,
                  eDecodeType &decode, tResourcePosition &comp_size, tResourcePosition &real_size, std::string &err);
    bool ReadData(eResourceType type, tResourceId index, tResourcePosition position, tResouceData &data, std::string &err);

    inline bool CopyTo(cResourcePackage &destination, std::string err) { return cFile::CopyTo(destination, err); }
    inline bool CopyTo(std::streampos position, cResourcePackage &destination, std::string err) { return cFile::CopyTo(position, destination, err); }

private:
    using tDecodeUPtr = std::unique_ptr<cDecode>;
    using tDecoders   = std::map<eDecodeType, tDecodeUPtr>;

private:
    tDecoders m_decoders;

    cDecode* Decoder(eDecodeType id, std::string &err);
};
