#include <stdafx.h>
#include "File.h"

using namespace std;

bool File::Open(const char *filename, ostream &err)
{
    if(!filename || !*filename)
    {
        ERROR_TO_STREAM(err) << "filename is empty\n";
        return false;
    }

    if(m_stream.is_open())
    {
        if(m_filename.compare(filename) == 0)
        {
            return true;
        }

        ERROR_TO_STREAM(err) << "'" << m_filename.c_str() << "' is opening instead '" << filename << "'\n";
        return false;
    }

    m_stream.open(filename, m_mode);
    if(!m_stream)
    {
        ERROR_TO_STREAM(err) << "'" << filename << "' is inexistent\n";
        return false;
    }

    m_stream.imbue(locale::classic());
    m_filename.assign(filename);
    return true;
}

void File::Close()
{
    m_filename.clear();
    if(m_stream.is_open())
        m_stream.close();
}

bool File::Read(Data &buffer, Data::size_type size, ostream &err)
{
    if(!m_stream)
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

    streamsize ss_size = streamsize(size);
    m_stream.read(reinterpret_cast<char *>(buffer.data()), ss_size);
    if(m_stream.gcount() != ss_size)
    {
        ERROR_TO_STREAM(err) << "binary data doesn't read\n";
        return false;
    }

    return true;
}
/*
bool File::CopyTo(File &destination, ostream &err)
{
    try
    {
        m_buffer.resize(s_buffer_size);
    }
    catch (bad_alloc exception)
    {
        ERROR_TO_STREAM(err) << exception.what() << "\n";
        return false;
    }

    while(m_stream)
    {
        m_stream.read(reinterpret_cast<char *>(m_buffer.data()), s_buffer_size);
        destination.m_stream.write(reinterpret_cast<char *>(m_buffer.data()), m_stream.gcount());
    }

    return true;
}

bool File::CopyTo(streampos position, File &destination, ostream &err)
{
    streampos current_position = m_stream.tellg();
    if(current_position > position)
    {

        ERROR_TO_STREAM(err) << "copying isn't possible, because current position is greater than position at the end copying\n";
        return false;
    }

    streamsize data_size  = position - current_position, ss_buffer_size = s_buffer_size;
    streamsize block_size = (data_size < ss_buffer_size) ? data_size : ss_buffer_size;

    try
    {
        m_buffer.resize(BinaryData::size_type(block_size));
    }
    catch (bad_alloc exception)
    {
        ERROR_TO_STREAM(err) << exception.what() << "\n";
        return false;
    }

    while(block_size)
    {
        m_stream.read(reinterpret_cast<char *>(m_buffer.data()), block_size);
        if(m_stream.gcount() != block_size)
        {
            ERROR_TO_STREAM(err) << "size of source file is less than size of destination file\n";
            return false;
        }
        destination.m_stream.write(reinterpret_cast<char *>(m_buffer.data()), block_size);

        data_size -= block_size;
        if(block_size > data_size)
            block_size = data_size;
    }

    return true;
}
*/
