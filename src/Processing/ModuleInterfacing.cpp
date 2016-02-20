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

    return 0;
}

int Application::PrepareOutput()
{
    // Process flat view data, if supported by both input and output module
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && OMF_ISSET(m_outputModuleFeatures, OMF_FLAT_PROFILE))
    {
        // at first, sort by call count - that's our secondary criteria
        std::sort(m_data->flatProfile.begin(), m_data->flatProfile.end(), FlatProfileCallCountSortPredicate());

        // then, sort by time spent, and use stable sort to not scramble already sorted entires
        // within same "bucket" of time quantum
        std::stable_sort(m_data->flatProfile.begin(), m_data->flatProfile.end(), FlatProfileTimeSortPredicate());

        double totalTime = 0.0;

        // sum of time spent in whole program
        for (int i = 0; i < m_data->flatProfile.size(); i++)
            totalTime += m_data->flatProfile[i].timeTotal;

        // calculate additional fields
        for (int i = 0; i < m_data->flatProfile.size(); i++)
        {
            if (totalTime > 0.0)
                m_data->flatProfile[i].timeTotalPct = m_data->flatProfile[i].timeTotal / totalTime;
            else
                m_data->flatProfile[i].timeTotalPct = 0.0;
        }
    }

    return 0;
}

int Application::ProceedOutput()
{
    m_outputModule->VisualizeData(m_data);

    return 0;
}
