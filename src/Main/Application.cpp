#include "General.h"
#include "InputModule.h"
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

    if (!CreateInputModuleHandle())
        return false;

    return true;
}

bool Application::CreateInputModuleHandle()
{
    // TODO: allow specifying module path?
    // TODO: search in both local and "global" library search paths (on linux use ./libname.so and if failed, libname.so)

    std::string inputModuleName = std::string("./") + std::string(INPUT_MODULE_NAME_PREFIX) + GetStringOption(CLIOPT_INPUT_MODULE);
    std::string inputModuleFile = inputModuleName + std::string(SHARED_LIBRARY_SUFFIX);

#ifdef _WIN32
    m_inputModuleHandle = LoadLibrary(inputModuleFile.c_str());
#else
    m_inputModuleHandle = dlopen(inputModuleFile.c_str(), RTLD_LAZY);
#endif
    if (!m_inputModuleHandle)
    {
        sLog->Error("Could not load module %s!", inputModuleName.c_str());
        return false;
    }

#ifdef _WIN32
    RegisterLogFunc registerLogDllFunc = (RegisterLogFunc)GetProcAddress(m_inputModuleHandle, "RegisterLogger");
#else
    RegisterLogFunc registerLogDllFunc = (RegisterLogFunc)dlsym(m_inputModuleHandle, "RegisterLogger");
#endif
    if (!registerLogDllFunc)
    {
        sLog->Error("Input module does not recognize RegisterLogger function!");
        return false;
    }

    registerLogDllFunc(DllLogFunc);

#ifdef _WIN32
    InputModuleCreateFunc createInputModuleDllFunc = (InputModuleCreateFunc)GetProcAddress(m_inputModuleHandle, "CreateInputModule");
#else
    InputModuleCreateFunc createInputModuleDllFunc = (InputModuleCreateFunc)dlsym(m_inputModuleHandle, "CreateInputModule");
#endif
    if (!createInputModuleDllFunc)
    {
        sLog->Error("Input module does not recognize CreateInputModule function!");
        return false;
    }

    m_inputModule = createInputModuleDllFunc();
    if (!m_inputModule)
    {
        sLog->Error("Could not create InputModule instance!");
        return false;
    }

    return true;
}

int Application::Run()
{
    m_inputModule->LoadFile(GetStringOption(CLIOPT_INPUT_PATH).c_str());

    return 0;
}
