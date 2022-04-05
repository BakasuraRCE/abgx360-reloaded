#pragma once

#if (defined(_WIN32) || defined(__WIN32__)) && !defined(WIN32)
    #define WIN32
#endif

#include <stdio.h>
#include <string.h>

#ifdef WIN32
    #include <Windows.h>      // for GetTickCount, SetConsoleTextAttribute, ReadFile, RegOpenKeyEx, RegQueryValueEx, CreateFile, SetFilePointerEx, SetEndOfFile, ...
#endif

#ifdef __cplusplus
    #define EXTERNC extern "C"
#else
    #define EXTERNC
#endif

EXTERNC int processdirectory(char *rootdir, char **filenames, bool *blockdevice, bool matchonly, int matchfiles, char **matchfilelist, char *s);

#undef EXTERNC