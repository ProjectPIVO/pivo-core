#include "General.h"
#include "InputModuleFeatures.h"
#include "InputModule.h"
#include "OutputModuleFeatures.h"
#include "OutputModule.h"
#include "UnitIdentifiers.h"
#include "FlatProfileStructs.h"
#include "NormalizedData.h"
#include "UnitIdentifiers.h"
#include "InputModuleFeatures.h"
#include "Application.h"
#include "Graph.h"

#include <stack>
#include <algorithm>

int Application::InitInput()
{
    // use module load method to load specified files (profiling output and binary file, if specified)
    if (!m_inputModule->LoadFile(GetStringOption(CLIOPT_INPUT_PATH).c_str(), GetStringOption(CLIOPT_INPUT_BINARY_PATH).c_str()))
        return 1;

    // request features set
    m_inputModule->ReportFeatures(m_inputModuleFeatures);

    // request class and function tables
    m_inputModule->GetClassTable(m_classTable);
    m_inputModule->GetFunctionTable(m_functionTable);

    return 0;
}

int Application::InitOutput()
{
    // TODO: some validity check calls to output module, for i.e. present templates, etc.

    m_outputModule->ReportFeatures(m_outputModuleFeatures);

    return 0;
}

int Application::GatherData()
{
    m_data = new NormalizedData;

    m_data->classTable.assign(m_classTable.begin(), m_classTable.end());
    m_data->functionTable.assign(m_functionTable.begin(), m_functionTable.end());

    // set input file (directory) path
    m_data->inputProfDataFile = GetStringOption(CLIOPT_INPUT_PATH);
    // set input binary file path
    if (IsOptionSet(CLIOPT_INPUT_BINARY_PATH))
        m_data->inputBinaryFile = GetStringOption(CLIOPT_INPUT_BINARY_PATH);

    // resolve flat profile, if available
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE))
        m_inputModule->GetFlatProfileData(m_data->flatProfile);

    // resolve call graph, if available
    if (IMF_ISSET(m_inputModuleFeatures, IMF_CALL_GRAPH))
        m_inputModule->GetCallGraphMap(m_data->callGraph);

    return 0;
}

int Application::PrepareOutput()
{
    double totalTime = 0.0;

    // Process flat view data time percentage, if supported by both input and output module
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && OMF_ISSET(m_outputModuleFeatures, OMF_FLAT_PROFILE))
    {
        // sum of time spent in whole program
        for (int i = 0; i < m_data->flatProfile.size(); i++)
            totalTime += m_data->flatProfile[i].timeTotal;

        // calculate additional fields - percentage of time spent, etc.
        for (int i = 0; i < m_data->flatProfile.size(); i++)
        {
            if (totalTime > 0.0)
                m_data->flatProfile[i].timeTotalPct = m_data->flatProfile[i].timeTotal / totalTime;
            else
                m_data->flatProfile[i].timeTotalPct = 0.0;

            // inclusive time is initially total time spent in this function call
            m_data->flatProfile[i].timeTotalInclusive = m_data->flatProfile[i].timeTotal;
            m_data->flatProfile[i].timeTotalInclusivePct = 0.0;
        }
    }

    // Deduce inclusive time, if both flat profile and call graph are supported
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && IMF_ISSET(m_inputModuleFeatures, IMF_CALL_GRAPH))
    {
        Graph cgraph;

        // go through callGraph arcs, and build directed graph
        for (auto itr : m_data->callGraph)
        {
            // add nodes in reverse direction - this graph means "node X adds time to node Y"
            for (auto sitr : itr.second)
                cgraph.addEdge(sitr.first, itr.first);
        }

        // find strongly connected components to detect i.e. recursion
        cgraph.findSCC();

        // retrieve nodes for future operations
        NodeSet nodes;
        nodeid_t node;
        cgraph.getNodes(&nodes);

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

        NodeSet inStackNodes;

        // define macro for detecting node presence in already traversed nodes
        #define IS_IN_STACK(x) (inStackNodes.find(x) != inStackNodes.end())

        for (auto itr : nodes)
        {
            node = itr;

            inStackNodes.clear();
            // close our node to not be traversed again during this iteration
            inStackNodes.insert(node);

            // this time will be distributed during DFS traversal
            double timesrc = m_data->flatProfile[node].timeTotal;

            // find adjacent nodes and push them
            NodeSet* adj = cgraph.getAdjacentNodes(node);
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

                inStackNodes.erase(adnode);

                // push adjacent nodes onto stack
                NodeSet* adj = cgraph.getAdjacentNodes(adnode);
                for (auto aitr : *adj)
                {
                    // if they're already on stack, ignore them (for now)
                    if (IS_IN_STACK(aitr))
                        continue;

                    // push to stack, and calculate multiplier
                    inStackNodes.insert(aitr);
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
        for (int i = 0; i < m_data->flatProfile.size(); i++)
        {
            if (m_data->flatProfile[i].timeTotalInclusive > 0)
                m_data->flatProfile[i].timeTotalInclusivePct = m_data->flatProfile[i].timeTotalInclusive / totalTime;
        }
    }

    // Process flat view data function order, if supported by both input and output module
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && OMF_ISSET(m_outputModuleFeatures, OMF_FLAT_PROFILE))
    {
        // at first, sort by call count - that's our secondary criteria
        std::sort(m_data->flatProfile.begin(), m_data->flatProfile.end(), FlatProfileCallCountSortPredicate());

        // then, sort by time spent, and use stable sort to not scramble already sorted entires
        // within same "bucket" of time quantum
        std::stable_sort(m_data->flatProfile.begin(), m_data->flatProfile.end(), FlatProfileTimeSortPredicate());
    }

    return 0;
}

int Application::ProceedOutput()
{
    m_outputModule->VisualizeData(m_data);

    return 0;
}
