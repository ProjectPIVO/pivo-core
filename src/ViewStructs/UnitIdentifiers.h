#ifndef PIVO_UNIT_IDENTIFIERS_H
#define PIVO_UNIT_IDENTIFIERS_H

#define NO_CLASS -1

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
