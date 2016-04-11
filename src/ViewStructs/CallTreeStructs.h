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

#ifndef PIVO_CALLTREE_STRUCTS_H
#define PIVO_CALLTREE_STRUCTS_H

struct CallTreeNode
{
    // function ID from function table
    uint32_t functionId;
    // total time (or samples) spent here
    double timeTotal;
    // total percentage of time (or samples) spent here
    double timeTotalPct;
    // profiling samples count (samples or calls)
    uint64_t sampleCount;

    // link to parent node
    CallTreeNode* parent;
    // key = functionId of children (for faster lookup), value = children record
    std::map<uint32_t, CallTreeNode*> children;
};

// set of "root nodes" for call tree
typedef std::map<uint32_t, CallTreeNode*> CallTreeMap;

#endif
