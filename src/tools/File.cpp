#include <stdafx.h>
#include "File.h"

using namespace std;

bool File::Open(const char *filename, ostream &err)
{
    Close();

    if (!filename || !*filename)
    {
        ERROR_TO_STREAM(err) << "filename is empty\n";
        return false;
    }

    if (m_stream.is_open())
    {
        if (m_filename.compare(filename) == 0)
        {
            return true;
        }

        ERROR_TO_STREAM(err) << "'" << m_filename.c_str() << "' open instead '" << filename << "'\n";
        return false;
    }

    m_filename.assign(filename);
    m_stream.open(m_filename, m_mode);
    if (!m_stream)
    {
        ERROR_TO_STREAM(err) << "'" << filename << "' is inexistent\n";
        return false;
    }

    return true;
}

void File::Close()
{
    if (m_stream.is_open())
    {
        m_stream.close();
    }
    m_filename.clear();
}

bool File::Read(Data &buffer, Data::size_type size, ostream &err) noexcept
{
    if (!m_stream)
    {
        ERROR_TO_STREAM(err) << "file isn't opened\n";
        return false;
    }

    if(size > LONG_MAX)
    {
        ERROR_TO_STREAM(err) << "Size is out of range\n";
        return false;
    }

    try
    {
        buffer.resize(size);
    }
    catch (bad_alloc exception)
    {
        ERROR_TO_STREAM(err) << exception.what() << "\n";
        return false;
    }

    streamsize ss_size = static_cast<streamsize>(size);
    try
    {
        m_stream.read(reinterpret_cast<char *>(buffer.data()), ss_size);
        if(m_stream.gcount() != ss_size)
        {
            throw ios_base::failure("binary data doesn't read"_s);
        }
    }
    catch (const ios_base::failure& ex)
    {
        ERROR_TO_STREAM(err) << ex.what() << "\n";
        return false;
    }

    return true;
}
