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
#include "Log.h"
#include "Analyzer.h"

#include <sstream>
#include <iomanip>

int Application::InitInput()
{
    sLog->Info("Initializing input module");

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
    sLog->Info("Initializing output module");

    // TODO: some validity check calls to output module, for i.e. present templates, etc.

    m_outputModule->ReportFeatures(m_outputModuleFeatures);

    return 0;
}

int Application::GatherData()
{
    sLog->Info("Gathering data from input module");

    m_data = new NormalizedData;

    m_data->classTable.assign(m_classTable.begin(), m_classTable.end());
    m_data->functionTable.assign(m_functionTable.begin(), m_functionTable.end());

    // set input file (directory) path
    m_data->inputProfDataFile = GetStringOption(CLIOPT_INPUT_PATH);
    // set input binary file path
    if (IsOptionSet(CLIOPT_INPUT_BINARY_PATH))
        m_data->inputBinaryFile = GetStringOption(CLIOPT_INPUT_BINARY_PATH);

    m_data->outputPath = GetStringOption(CLIOPT_OUTPUT_PATH);

    // default unit is samples
    m_data->profilingUnit = PU_SAMPLES;
    // some profilers (such as gprof) may use second as unit
    if (IMF_ISSET(m_inputModuleFeatures, IMF_USE_SECONDS))
    {
        sLog->Verbose("Switching profiling unit to seconds");
        m_data->profilingUnit = PU_TIME;
    }

    // resolve flat profile, if available
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE))
    {
        sLog->Verbose("Retrieving flat profile data");
        m_inputModule->GetFlatProfileData(m_data->flatProfile);
    }

    // resolve call graph, if available
    if (IMF_ISSET(m_inputModuleFeatures, IMF_CALL_GRAPH))
    {
        sLog->Verbose("Retrieving call graph data");
        m_inputModule->GetCallGraphMap(m_data->callGraph);
    }

    // resolve call tree, if available
    if (IMF_ISSET(m_inputModuleFeatures, IMF_CALL_TREE))
    {
        sLog->Verbose("Retrieving call tree data");
        m_inputModule->GetCallTreeMap(m_data->callTree);
    }

    return 0;
}

int Application::PrepareOutput()
{
    sLog->Info("Analyzing gathered data");

    Analyzer dataAnalyzer(m_data);

    double totalTime = 0.0;

    // Process flat view data time percentage, if supported by both input and output module
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && OMF_ISSET(m_outputModuleFeatures, OMF_FLAT_PROFILE))
        totalTime = dataAnalyzer.InitializeTimeAnalysis(!IMF_ISSET(m_inputModuleFeatures, IMF_INCLUSIVE_TIME));

    // Deduce inclusive time, if both flat profile and call graph are supported, AND the module itself does not have support for this feature
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && IMF_ISSET(m_inputModuleFeatures, IMF_CALL_GRAPH) && !IMF_ISSET(m_inputModuleFeatures, IMF_INCLUSIVE_TIME))
        dataAnalyzer.CalculateInclusiveTime();

    // Process flat view data function order, if supported by both input and output module
    if (IMF_ISSET(m_inputModuleFeatures, IMF_FLAT_PROFILE) && OMF_ISSET(m_outputModuleFeatures, OMF_FLAT_PROFILE))
        dataAnalyzer.FinalizeFlatProfileTable();

    sLog->Verbose("Filling summary block");

    // Fill basic info (summary) map
    m_data->basicInfo["Binary file"] = GetStringOption(CLIOPT_INPUT_BINARY_PATH);
    m_data->basicInfo["Input path"] = GetStringOption(CLIOPT_INPUT_PATH);
    m_data->basicInfo["Analyzed function count"] = std::to_string(m_functionTable.size());

    switch (m_data->profilingUnit)
    {
        case PU_TIME:
        {
            std::ostringstream out;
            out << std::setprecision(2) << std::fixed << totalTime;
            m_data->basicInfo["Total execution time"] = out.str() + "s";
            break;
        }
        case PU_SAMPLES:
        {
            m_data->basicInfo["Total samples"] = std::to_string((uint64_t)totalTime);
            break;
        }
        default:
            break;
    }

    m_data->basicInfo["Input module name"] = m_inputModule->ReportName();
    m_data->basicInfo["Input module version"] = m_inputModule->ReportVersion();
    m_data->basicInfo["Output module name"] = m_outputModule->ReportName();
    m_data->basicInfo["Output module version"] = m_outputModule->ReportVersion();

    return 0;
}

int Application::ProceedOutput()
{
    sLog->Info("Passing gathered data to output module");

    if (!m_outputModule->VisualizeData(m_data))
        return 1;

    return 0;
}
