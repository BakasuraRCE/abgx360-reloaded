#pragma once

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

EXTERNC void ok1(int ok);

#undef EXTERNC
