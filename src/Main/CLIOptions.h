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

#ifndef PIVO_CLIOPTS_H
#define PIVO_CLIOPTS_H

enum CommandLineOptionType
{
    CLIOPTYPE_STRING = 0,
    CLIOPTYPE_INT,
    CLIOPTYPE_BOOL,
    MAX_CLIOPTYPE
};

union CommandLineOptionUnion
{
    char* asString;
    int64_t asInt;
    // bool asBool; // we will use asInt value 0 / 1 to recognize false / true boolean value
};

struct CommandLineOptionValue
{
    CommandLineOptionType  type;
    CommandLineOptionUnion value;
};

#endif
