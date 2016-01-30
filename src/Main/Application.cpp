#include "General.h"
#include "Application.h"
#include "Log.h"
#include "Version.h"

Application::Application()
{
    //
}

Application::~Application()
{
    //
}

void Application::InitCommandLineOpts()
{
    // In this method, we define accepted command line parameters
    // and set them default values, optionally aliases

    AddDefaultOptionString("--input-module", CLIOPT_INPUT_MODULE, UNDEFINED_STR);
    AddOptionNameAlias("-im", CLIOPT_INPUT_MODULE);

    AddDefaultOptionString("--output-module", CLIOPT_OUTPUT_MODULE, UNDEFINED_STR);
    AddOptionNameAlias("-om", CLIOPT_OUTPUT_MODULE);

    AddDefaultOptionString("--log-file", CLIOPT_LOG_FILE, UNDEFINED_STR);
    AddOptionNameAlias("-lf", CLIOPT_LOG_FILE);

    AddDefaultOptionInt("--log-level", CLIOPT_LOG_LEVEL, LOG_ERROR);
    AddOptionNameAlias("-ll", CLIOPT_LOG_LEVEL);

    AddDefaultOptionString("--input", CLIOPT_INPUT_PATH, UNDEFINED_STR);
    AddOptionNameAlias("-i", CLIOPT_INPUT_PATH);

    AddDefaultOptionString("--output", CLIOPT_OUTPUT_PATH, UNDEFINED_STR);
    AddOptionNameAlias("-o", CLIOPT_OUTPUT_PATH);

    AddDefaultOptionBool("--silent", CLIOPT_SILENT, false);
    AddOptionNameAlias("-s", CLIOPT_SILENT);
}

void printProgramInfo()
{
    std::cout << "PIVO - Profiler-Independent Visual Output, ";
    std::cout << "version " << PIVO_CORE_VERSION_MAJOR << "." << PIVO_CORE_VERSION_MINOR;

    if (strlen(PIVO_CORE_VERSION_NOTE) > 0)
        std::cout << "-" << PIVO_CORE_VERSION_NOTE << std::endl;
    else
        std::cout << std::endl;

    std::cout << "Copyright (c) 2015-2016, Martin Ubl, ublm@students.zcu.cz" << std::endl;
    std::cout << "University of West Bohemia, Faculty of Applied Sciences" << std::endl << "Department of Computer Science and Engineering" << std::endl;

    std::cout << std::endl;
}

bool Application::Init(int argc, char** argv)
{
    // init log to display all errors for now
    // later we will reconfigure it to take command line parameters seriously
    sLog->SetSilent(false);
    sLog->SetLogLevel(LOG_ERROR);

    // init default command line parameter values
    InitCommandLineOpts();

    // load real command line parameters
    if (!ParseCommandLineOpts(argc, argv))
        return false;

    // if not in silent mode, print program info (visible in all modes)
    if (!GetBoolOption(CLIOPT_SILENT))
        printProgramInfo();

    // set log parameters retained from command line
    sLog->SetSilent(GetBoolOption(CLIOPT_SILENT));
    sLog->SetLogLevel((int)GetIntOption(CLIOPT_LOG_LEVEL));

    if (IsOptionSet(CLIOPT_LOG_FILE))
    {
        if (sLog->SetLogFile(GetStringOption(CLIOPT_LOG_FILE).c_str()))
            sLog->Info("Using log file %s", sLog->GetLogFile());
        else
            sLog->Error("Could not open file %s for writing!", GetStringOption(CLIOPT_LOG_FILE).c_str());
    }

    // TODO: verify input file has been specified

    // TODO: verify input file exists

    // TODO: verify input and output module has been specified

    // TODO: verify presence of input and output modules

    return true;
}

int Application::Run()
{
    return 0;
}
