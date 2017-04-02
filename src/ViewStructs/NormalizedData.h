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

#ifndef PIVO_NORMALIZED_DATA_H
#define PIVO_NORMALIZED_DATA_H

#include "UnitIdentifiers.h"
#include "FlatProfileStructs.h"
#include "CallGraphStructs.h"
#include "CallTreeStructs.h"
#include "HeatMapStructs.h"

// Structure consisting of all gathered data, later passed to output module
struct NormalizedData
{
    // name of file (or folder) containing profiling data
    std::string inputProfDataFile;
    // name of binary file (if supplied)
    std::string inputBinaryFile;

    // basic information about profiling session, etc.
    std::map<std::string, std::string> basicInfo;

    // output path
    std::string outputPath;

    // units of profiling
    ProfilingUnits profilingUnit;

    // class table
    std::vector<ClassEntry> classTable;
    // function table
    std::vector<FunctionEntry> functionTable;

    // flat profile data (containing no context, just times and call counts with functions)
    std::vector<FlatProfileRecord> flatProfile;
    // call graph data (containing caller-callee data with call count)
    CallGraphMap callGraph;
    // call tree data (containing tree structure with times count)
    CallTreeMap callTree;
    // heat map data (containing vector of histograms)
    TimeHistogramVector heatMapData;
};

#endif
