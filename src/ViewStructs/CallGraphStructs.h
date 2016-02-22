#ifndef PIVO_CALLGRAPH_STRUCTS_H
#define PIVO_CALLGRAPH_STRUCTS_H

// do not confuse call graph with call tree - the call graph does not
// support full call hierarchy, and serves just as overview of most used
// call paths to determine hot path (critical call path, where the
// program spent a lot of time during execution)

// primary key = caller, secondary key = callee, value = call count
typedef std::map<uint32_t, std::map<uint32_t, uint64_t>> CallGraphMap;

#endif
