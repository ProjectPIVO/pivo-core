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

#ifndef PIVO_UNIT_IDENTIFIERS_H
#define PIVO_UNIT_IDENTIFIERS_H

#define NO_CLASS -1

enum FunctionEntryType
{
    FET_TEXT = 't',       // generally the only type we are interested into - functions defined in measured application
    FET_TEXT_2 = 'T',     // second symbol type for text section
    FET_WEAK = 'w',       // weak symbol
    FET_WEAK_2 = 'W',     // another weak symbol type
    FET_MISC = 'x',       // functions from other parts of system, glibc, syscalls, ...
    FET_READONLY = 'r',   // symbols in read-only section
    FET_READONLY_2 = 'R',
    FET_BSS = 'b',        // symbols in BSS section
    FET_BSS2 = 'B',
    FET_KERNEL = '9',     // custom; symbols loaded as kernel modules
    // for now, there are no more types we are interested into

    FET_DONTCARE = ' '    // special type used for "do not override" option
};

enum ProfilingUnits
{
    PU_SAMPLES = 0,          // default - number of samples
    PU_TIME = 1,             // time in seconds
    MAX_PU
};

struct ClassEntry
{
    // Class/struct name
    std::string name;
};

struct FunctionEntry
{
    // entry address
    uint64_t address;
    // address scaled according to i.e. sampling frequency (may not be used)
    uint64_t scaled_address;
    // Function/method name
    std::string name;
    // If function (method) belongs to class, this is the index to class table
    int32_t classId;
    // type of function (mapped using symbol output)
    FunctionEntryType functionType;
};

// structure used for sorting std::vector of FunctionEntry by address
struct FunctionEntrySortPredicate
{
    inline bool operator() (const FunctionEntry& a, const FunctionEntry& b)
    {
        return (a.address < b.address);
    }
};

#endif
