/**
 * Copyright (C) 2016 Martin Ubl <http://pivo.kennny.cz>
 *
 * This file is part of PIVO suite.
 *
 * PIVO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIVO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PIVO. If not, see <http://www.gnu.org/licenses/>.
 **/

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
