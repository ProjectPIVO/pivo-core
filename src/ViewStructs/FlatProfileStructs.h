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

#ifndef PIVO_FLAT_PROFILE_STRUCTS_H
#define PIVO_FLAT_PROFILE_STRUCTS_H

struct FlatProfileRecord
{
    // function index in function table
    uint32_t functionId;
    // time percentage spent in this function
    double timeTotalPct;
    // absolute time spent here (dimension is set elsewhere)
    double timeTotal;
    // call count (may have been recorded as well, otherwise zero)
    uint32_t callCount;
    // inclusive time spent by execution
    double timeTotalInclusive;
    // inclusive time percentage spent by execution
    double timeTotalInclusivePct;
};

// structure used for sorting std::vector of FlatProfileRecord by time spent
struct FlatProfileTimeSortPredicate
{
    inline bool operator() (const FlatProfileRecord& a, const FlatProfileRecord& b)
    {
        return (a.timeTotal > b.timeTotal);
    }
};

// structure used for sorting std::vector of FlatProfileRecord by call count
struct FlatProfileCallCountSortPredicate
{
    inline bool operator() (const FlatProfileRecord& a, const FlatProfileRecord& b)
    {
        return (a.callCount > b.callCount);
    }
};

#endif
