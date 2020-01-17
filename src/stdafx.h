#ifndef stdafx_h
#define stdafx_h

// shutdown extra warnings of MSVC
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

// add C includes here

#ifdef __cplusplus
/*
// add C++ includes here
#include <algorithm>
*/
#include <climits>  //+
#include <iostream> //+
#include <fstream>  //+
#include <list>
#include <map>
#include <set>
#include <stdexcept>//+
#include <string>
#include <stdint.h> //+
#include <vector>   //+

// add Qt modules here
//#include <QtWidgets>

//add common prject's module here

//add common variables here
#endif // __cplusplus

#define CLASSNAME_TO_STREAM(stream)                                             \
{                                                                               \
    const char *classname_ptr = typeid(*this).name();                           \
    while((*classname_ptr <= '9') && (*classname_ptr >= '0')) ++classname_ptr;  \
    stream << classname_ptr;                                                    \
}                                                                               \
stream

#define CLASSFUNC_TO_STREAM(stream)                                                                                                 \
CLASSNAME_TO_STREAM(stream) <<  "::" << __func__ << "(), \"" << __FILE__  << "\"" << ", " << static_cast<int>(__LINE__) << " line"; \
stream

#define ERROR_TO_STREAM(stream)         \
CLASSFUNC_TO_STREAM(stream) << ":\n\t"; \
stream

/// @brief User-defined suffix operator _s for more efficiently conversion of string literal to std::string
inline std::string operator ""_s(const char *text, std::size_t size)
{
   return std::string(text, size);
}

#endif // stdafx_h
