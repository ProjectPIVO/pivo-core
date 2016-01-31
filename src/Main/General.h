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
#else
#include <unistd.h>
#include <dlfcn.h>
#endif

#define UNDEFINED_STR "__undefined__"
#define UNDEFINED_INT INT_MAX

#define DLL_EXPORT_API __declspec(dllexport)

#endif
