/**
 * Copyright (C) 2016 Martin Ubl <http://pivo.kennny.cz>
 *
 * This file is part of PIVO suite.
 *
 * PIVO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIVO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PIVO. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef PIVO_GENERAL_H
#define PIVO_GENERAL_H

#include <iostream>
#include <stdint.h>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <set>
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
