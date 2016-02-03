#ifndef PIVO_FLAT_PROFILE_STRUCTS_H
#define PIVO_FLAT_PROFILE_STRUCTS_H

struct FlatProfileRecord
{
    // function index in function table
    uint32_t functionId;
    // time percentage spent in this function
    float timeTotalPct;
    // absolute time spent here (dimension is set elsewhere)
    uint32_t timeTotal;
    // call count (may have been recorded as well, otherwise zero)
    uint32_t callCount;
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
