#ifndef PIVO_GENERAL_H
#define PIVO_GENERAL_H

#include <iostream>
#include <stdint.h>
#include <list>
#include <map>
#include <string>
#include <cstring>
#include <limits.h>

#ifdef _WIN32
#include <Windows.h>
#define DLL_EXPORT_API __declspec(dllexport)
#else
#include <unistd.h>
#include <dlfcn.h>
#define DLL_EXPORT_API 
#endif

#ifndef nmax
#define nmax(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef nmin
#define nmin(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#define UNDEFINED_STR "__undefined__"
#define UNDEFINED_INT INT_MAX

#endif
