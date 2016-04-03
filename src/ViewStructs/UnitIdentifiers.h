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
