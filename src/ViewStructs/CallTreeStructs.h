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
