#ifndef PIVO_NORMALIZED_DATA_H
#define PIVO_NORMALIZED_DATA_H

// Structure consisting of all gathered data, later passed to output module
struct NormalizedData
{
    // name of file (or folder) containing profiling data
    std::string inputProfDataFile;
    // name of binary file (if supplied)
    std::string inputBinaryFile;

    // flat profile data (containing no context, just times and call counts with functions)
    std::vector<FlatProfileRecord> flatProfile;
};

#endif