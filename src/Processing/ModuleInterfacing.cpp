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
    //

    return 0;
}

int Application::ProceedOutput()
{
    m_outputModule->VisualizeData(m_data);

    return 0;
}
