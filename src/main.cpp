#include "stdafx.h"
#include "tools/File.h"

//#include "ResourceImpl.h"
//#include "entity/Font.h"

//#include <algorithm>
//#include <set>
//#include <chrono>

#include <qapplication.h>
#include <qpushbutton.h>

using namespace std;

/*
int output(const cResource &resource)
{
    tResourceId   id;
    eResourceType type;
    for(tResourceType type_id = s_resource_type_first; type_id != s_resource_type_last; ++type_id)
    {
        type = static_cast<eResourceType>(type_id);
        const cResourcePage &page = resource.At(type);
        for(cResourcePage::tResourcePtr description_ptr : page)
        {
            id = page.Id(description_ptr);
            if(id == s_resource_id_none)
            {
                printf("ERROR: index of data is incorrectly");
                return 2;
            }

            printf("position: %06x; type: %02x; index: %04x\r\n", description_ptr->position, type_id, id);
        }
    }

    return 0;
}

void output(const tResouceData &data)
{
    bool is_separator = false;
    for(unsigned int i = 0, size = unsigned int(data.size()); i < size; ++i)
    {
        if(i % 8)             printf(" %02x", data[i]);
        else if(is_separator) { printf("-%02x", data[i]); is_separator = false; }
        else                  { printf("%s%08x: %02x", (i) ? "\r\n" : "", i, data[i]); is_separator = true; }
    }
}
*/

int main(int argc, char* argv[])
{
/*
    QApplication app(argc, argv);
    QPushButton hello( "Hello world!", 0 );
    hello.resize( 100, 30 );
    hello.show();
    return app.exec();
*/
    File file{ std::ios_base::in | std::ios_base::binary };
    file.Open("RESOURCE.MAP", std::cerr);
/*
    ResourceAPI resource;
    bool opened = resource.Open(nullptr, err);  // "d:\\projects\\Qt\\SCI\\bin\\RESOURCE.MAP"
    if(!opened)
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 1;
    };
//    output(resource);

    const tResouceData &data = resource.Get(eResourceType::font, 123, err);
    if(data.empty())
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 2;
    };
//    output(data);

/*
    Font font;
    if(!font.Import(data.data(), data.size(), err))
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 3;
    };

    if(!font.Empty())
    {
        auto width  = font[0].Width();
        auto height = font[0].Height();
        for(std::size_t s = 0, max = font.Total(); s < max; ++s)
        {
            width  = font[s].Width();
            height = font[s].Height();
            const Font::Character::Bitmap &data = font[s].Data();
            for(uint16_t i = 0, y = 0; y < height; ++y)
            {
                printf("%02u |", y);
                for(uint16_t x = 0; x < width; ++x, ++i) printf("%s", data[i] ? "X" : " ");
                printf("|\n");
            }
            printf("\n");
        }
    }
/**/

/*
    if(!resource.Insert(eResourceType::font, 123, data, err))
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 3;
    };
*//*
    if(!resource.Delete(eResourceType::font, 123, err))
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 4;
    };
/*
    if(!resource.Insert(eResourceType::font, 0, data, err))
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 5;
    };
*//*
    if(!resource.Save(err))
    {
        printf("ERROR: %s\r\n", err.c_str());
        return 6;
    };

/*
    QApplication app(argc, argv);
    return app.exec();
*/
    return 0;
}
