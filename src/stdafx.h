#ifndef stdafx_h
#define stdafx_h

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
#include <climits>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
/*
#include <memory>
*/
#include <set>
#include <string>
//#include <stdint.h>
#include <vector>
/*
// add Qt modules here
//#include <QtWidgets>

//add common prject's module here
#include <SCIFields.h>

//add common variables here
static const char* s_temp_dir = "temp/";
*/
#endif // __cplusplus

#define CLASSNAME_TO_STREAM(stream)                                             \
{                                                                               \
    const char *classname_ptr = typeid(*this).name();                           \
    while((*classname_ptr <= '9') && (*classname_ptr >= '0')) ++classname_ptr;  \
    stream << classname_ptr;                                                    \
}                                                                               \
stream

#define CLASSFUNC_TO_STREAM(stream)                                                                 \
CLASSNAME_TO_STREAM(stream) <<  "::" << __func__ << "(" << static_cast<int>(__LINE__) << " line)";  \
stream

#define ERROR_TO_STREAM(stream)         \
CLASSFUNC_TO_STREAM(stream) << ":\n\t"; \
stream

#endif // stdafx_h
