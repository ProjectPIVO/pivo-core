#ifndef PIVO_ANALYZER_H
#define PIVO_ANALYZER_H

#include "UnitIdentifiers.h"
#include "FlatProfileStructs.h"
#include "Graph.h"
#include "CallGraphStructs.h"
#include "CallTreeStructs.h"
#include "NormalizedData.h"

// Analyzer class for post-processing results from input module
class Analyzer
{
    public:
        Analyzer(NormalizedData* data);

        // Resets all time fields, prepares time calculation-related values, returns total time spent
        double InitializeTimeAnalysis(bool resetInclusive = false);
        // Calculates inclusive time based on call graph and flat profile data
        void CalculateInclusiveTime();
        // Finalizes flat profile table (presort, etc.)
        void FinalizeFlatProfileTable();

    protected:
        //

    private:
        // stored data structure
        NormalizedData* m_data;
        // total time stored for further calculations
        double m_totalTime;
        // stored call graph
        Graph m_graph;
};

#endif
