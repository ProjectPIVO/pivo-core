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

#ifndef PIVO_CALLGRAPH_STRUCTS_H
#define PIVO_CALLGRAPH_STRUCTS_H

// do not confuse call graph with call tree - the call graph does not
// support full call hierarchy, and serves just as overview of most used
// call paths to determine hot path (critical call path, where the
// program spent a lot of time during execution)

// primary key = caller, secondary key = callee, value = call count
typedef std::map<uint32_t, std::map<uint32_t, uint64_t>> CallGraphMap;

#endif
