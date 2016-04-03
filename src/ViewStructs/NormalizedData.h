#ifndef PIVO_NORMALIZED_DATA_H
#define PIVO_NORMALIZED_DATA_H

#include "UnitIdentifiers.h"
#include "FlatProfileStructs.h"
#include "CallGraphStructs.h"
#include "CallTreeStructs.h"

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
};

#endif
