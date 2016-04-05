#include "General.h"
#include "Analyzer.h"
#include "Log.h"

#include <stack>
#include <algorithm>

Analyzer::Analyzer(NormalizedData* data) : m_data(data)
{
}

double Analyzer::InitializeTimeAnalysis(bool resetInclusive)
{
    sLog->Verbose("Analyzing flat profile data");

    // sum of time spent in whole program
    for (size_t i = 0; i < m_data->flatProfile.size(); i++)
        m_totalTime += m_data->flatProfile[i].timeTotal;

    // calculate additional fields - percentage of time spent, etc.
    for (size_t i = 0; i < m_data->flatProfile.size(); i++)
    {
        if (m_totalTime > 0.0)
            m_data->flatProfile[i].timeTotalPct = m_data->flatProfile[i].timeTotal / m_totalTime;
        else
            m_data->flatProfile[i].timeTotalPct = 0.0;
    }

    // when the module itself does not support inclusive time calculation, nullify it and calculate it later
    if (resetInclusive)
    {
        for (size_t i = 0; i < m_data->flatProfile.size(); i++)
        {
            // inclusive time is initially total time spent in this function call
            m_data->flatProfile[i].timeTotalInclusive = m_data->flatProfile[i].timeTotal;
            m_data->flatProfile[i].timeTotalInclusivePct = 0.0;
        }
    }

    return m_totalTime;
}

void Analyzer::CalculateInclusiveTime()
{
    sLog->Verbose("Calculating inclusive time");

    // go through callGraph arcs, and build directed graph
    for (auto itr : m_data->callGraph)
    {
        // add nodes in reverse direction - this graph means "node X adds time to node Y"
        for (auto sitr : itr.second)
            m_graph.addEdge(sitr.first, itr.first);
    }

    // find strongly connected components to detect i.e. recursion
    m_graph.findSCC();

    // retrieve nodes for future operations
    NodeSet nodes;
    nodeid_t node;
    m_graph.getNodes(&nodes);

    /* TODO for far future: substitute this algorithm with some more efficient.
     * For now, the inclusive time value is resolved using DFS expanded from each node,
     * which is sufficiently quick for most cases; however, there are more efficient
     * algorithms, such as finding leaves (O(n)), and expanding i.e. using DFS for just leaves (O(n)),
     * but resulting time would not be much different, as we have to find those leaves first,
     * and due to count of graph nodes, it's not a priority to do that rather than DFS expand
     * for all nodes
     */

    // standard DFS stack holding node IDs (function IDs, indexes in function table and flat profile table)
    std::stack<nodeid_t> dfsStack;
    // multiplier stack for applying the right multiplicator during current DFS iteration
    std::stack<double> multiplierStack;

    NodeSet visitedNodes;

    // define macro for detecting node presence in already traversed nodes
    #define IS_VISITED(x) (visitedNodes.find(x) != visitedNodes.end())

    for (auto itr : nodes)
    {
        node = itr;

        visitedNodes.clear();
        // close our node to not be traversed again during this iteration
        visitedNodes.insert(node);

        // this time will be distributed during DFS traversal
        double timesrc = m_data->flatProfile[node].timeTotal;

        // find adjacent nodes and push them
        NodeSet* adj = m_graph.getAdjacentNodes(node);
        for (auto aitr : *adj)
        {
            dfsStack.push(aitr);
            multiplierStack.push(((double)m_data->callGraph[aitr][node] ) / ((double)m_data->flatProfile[node].callCount));
        }

        // DFS traversal loop
        while (!dfsStack.empty())
        {
            // pop node and its multiplier
            nodeid_t adnode = dfsStack.top();
            dfsStack.pop();
            double mult = multiplierStack.top();
            multiplierStack.pop();

            // push adjacent nodes onto stack
            NodeSet* adj = m_graph.getAdjacentNodes(adnode);
            for (auto aitr : *adj)
            {
                // if they're already on stack, ignore them (for now)
                if (IS_VISITED(aitr))
                    continue;

                // push to stack, and calculate multiplier
                visitedNodes.insert(aitr);
                dfsStack.push(aitr);
                multiplierStack.push(mult * ((double)m_data->callGraph[aitr][adnode] ) / ((double)m_data->flatProfile[adnode].callCount));
            }

            // add portion of time calculated during traversal
            m_data->flatProfile[adnode].timeTotalInclusive += timesrc * mult;
        }
    }

    // we won't need this macro anymore
    #undef IS_VISITED

    // go through all nodes and calculate percentage of inclusive time
    for (size_t i = 0; i < m_data->flatProfile.size(); i++)
    {
        if (m_data->flatProfile[i].timeTotalInclusive > 0)
            m_data->flatProfile[i].timeTotalInclusivePct = m_data->flatProfile[i].timeTotalInclusive / m_totalTime;
    }
}

void Analyzer::FinalizeFlatProfileTable()
{
    sLog->Verbose("Sorting flat profile vector");

    // at first, sort by call count - that's our secondary criteria
    std::sort(m_data->flatProfile.begin(), m_data->flatProfile.end(), FlatProfileCallCountSortPredicate());

    // then, sort by time spent, and use stable sort to not scramble already sorted entires
    // within same "bucket" of time quantum
    std::stable_sort(m_data->flatProfile.begin(), m_data->flatProfile.end(), FlatProfileTimeSortPredicate());
}
