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

#include "General.h"
#include "InputModule.h"
#include "OutputModule.h"
#include "UnitIdentifiers.h"
#include "FlatProfileStructs.h"
#include "NormalizedData.h"
#include "Application.h"
#include "Log.h"
#include "Version.h"

Application::Application()
{
    m_data = nullptr;
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

    AddDefaultOptionString("--binary", CLIOPT_INPUT_BINARY_PATH, "");
    AddOptionNameAlias("-b", CLIOPT_INPUT_BINARY_PATH);

    AddDefaultOptionString("--output", CLIOPT_OUTPUT_PATH, UNDEFINED_STR);
    AddOptionNameAlias("-o", CLIOPT_OUTPUT_PATH);

    AddDefaultOptionBool("--silent", CLIOPT_SILENT, false);
    AddOptionNameAlias("-s", CLIOPT_SILENT);

    AddDefaultOptionBool("--help", CLIOPT_PRINTHELP, false);
}

void printProgramInfo()
{
    std::cout << "PIVO - Profiler-Independent Visual Output, ";
    std::cout << "version " << PIVO_CORE_VERSION_MAJOR << "." << PIVO_CORE_VERSION_MINOR;

    if (strlen(PIVO_CORE_VERSION_NOTE) > 0)
        std::cout << "-" << PIVO_CORE_VERSION_NOTE;

    std::cout << std::endl;

    std::cout << "Copyright (c) 2015-2016, Martin Ubl, ublm@students.zcu.cz" << std::endl;
    std::cout << "University of West Bohemia, Faculty of Applied Sciences" << std::endl << "Department of Computer Science and Engineering" << std::endl;

    std::cout << std::endl;
}

void printProgramHelp(char* zeroarg)
{
    std::cout << "Usage: " << zeroarg << " -im <module> -om <module> -i <path> ..." << std::endl << std::endl;
    std::cout << "Valid parameters:" << std::endl
        << "\t-im <input module> - specifies input module (mandatory)" << std::endl
        << "\t-om <output module> - specifies output module (mandatory)" << std::endl
        << "\t-i <input module> - specifies input module (mandatory)" << std::endl
        << "\t-lf <file> - specifies log file (default: none)" << std::endl
        << "\t-ll <level> - specifies log level (0 = OFF, 1 = ERROR, 2 = WARNING, 3 = INFO, 4 = VERBOSE, 5 = DEBUG) (default: 2)" << std::endl
        << "\t-b <binary> - specifies application binary (default: none)" << std::endl
        << "\t-o <path> - specifies output path (default: current workdir)" << std::endl
        << "\t-s - silent, no output at all (default: off)" << std::endl
        << "\t--help - prints this message" << std::endl;
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

    if (GetBoolOption(CLIOPT_PRINTHELP))
    {
        printProgramHelp(argv[0]);
        return false;
    }

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

    // verify input file has been specified
    if (!IsOptionSet(CLIOPT_INPUT_PATH))
    {
        sLog->Error("Input file or directory was not specified! Use -i <path> to specify input path");
        return false;
    }

    // TODO: verify input file exists

    // verify input and output module has been specified
    if (!IsOptionSet(CLIOPT_INPUT_MODULE))
    {
        sLog->Error("Input module was not specified! Use -im <name> to choose one");
        return false;
    }
    if (!IsOptionSet(CLIOPT_OUTPUT_MODULE))
    {
        sLog->Error("Output module was not specified! Use -om <name> to choose one");
        return false;
    }

    // Create input and output module instance

    if (!CreateInputModuleHandle())
        return false;

    if (!CreateOutputModuleHandle())
        return false;

    return true;
}

bool Application::CreateInputModuleHandle()
{
    // TODO: allow specifying module path?
    // TODO: search in both local and "global" library search paths (on linux use ./libname.so and if failed, libname.so)

    std::string inputModuleName = std::string("./") + std::string(INPUT_MODULE_NAME_PREFIX) + GetStringOption(CLIOPT_INPUT_MODULE);
    std::string inputModuleFile = inputModuleName + std::string(SHARED_LIBRARY_SUFFIX);

    sLog->Verbose("Creating input module '%s'", GetStringOption(CLIOPT_INPUT_MODULE).c_str());
    sLog->Verbose("Input module path: %s", inputModuleFile.c_str());

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
        sLog->Error("Input module does not implement RegisterLogger function!");
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
        sLog->Error("Input module does not implement CreateInputModule function!");
        return false;
    }

    m_inputModule = createInputModuleDllFunc();
    if (!m_inputModule)
    {
        sLog->Error("Could not create InputModule instance!");
        return false;
    }

    sLog->Info("Input module '%s' successfully created", GetStringOption(CLIOPT_INPUT_MODULE).c_str());

    return true;
}

bool Application::CreateOutputModuleHandle()
{
    // TODO: allow specifying module path?
    // TODO: search in both local and "global" library search paths (on linux use ./libname.so and if failed, libname.so)

    std::string outputModuleName = std::string("./") + std::string(OUTPUT_MODULE_NAME_PREFIX) + GetStringOption(CLIOPT_OUTPUT_MODULE);
    std::string outputModuleFile = outputModuleName + std::string(SHARED_LIBRARY_SUFFIX);

    sLog->Verbose("Creating output module '%s'", GetStringOption(CLIOPT_OUTPUT_MODULE).c_str());
    sLog->Verbose("Output module path: %s", outputModuleFile.c_str());

#ifdef _WIN32
    m_outputModuleHandle = LoadLibrary(outputModuleFile.c_str());
#else
    m_outputModuleHandle = dlopen(outputModuleFile.c_str(), RTLD_LAZY);
#endif
    if (!m_outputModuleHandle)
    {
        sLog->Error("Could not load module %s!", outputModuleName.c_str());
        return false;
    }

#ifdef _WIN32
    RegisterLogFunc registerLogDllFunc = (RegisterLogFunc)GetProcAddress(m_outputModuleHandle, "RegisterLogger");
#else
    RegisterLogFunc registerLogDllFunc = (RegisterLogFunc)dlsym(m_outputModuleHandle, "RegisterLogger");
#endif
    if (!registerLogDllFunc)
    {
        sLog->Error("Output module does not implement RegisterLogger function!");
        return false;
    }

    registerLogDllFunc(DllLogFunc);

#ifdef _WIN32
    OutputModuleCreateFunc createOutputModuleDllFunc = (OutputModuleCreateFunc)GetProcAddress(m_outputModuleHandle, "CreateOutputModule");
#else
    OutputModuleCreateFunc createOutputModuleDllFunc = (OutputModuleCreateFunc)dlsym(m_outputModuleHandle, "CreateOutputModule");
#endif
    if (!createOutputModuleDllFunc)
    {
        sLog->Error("Output module does not implement CreateOutputModule function!");
        return false;
    }

    m_outputModule = createOutputModuleDllFunc();
    if (!m_outputModule)
    {
        sLog->Error("Could not create OutputModule instance!");
        return false;
    }

    sLog->Info("Output module '%s' successfully created", GetStringOption(CLIOPT_OUTPUT_MODULE).c_str());

    return true;
}

int Application::Run()
{
    if (InitInput() != 0 ||
        InitOutput() != 0 ||
        GatherData() != 0 ||
        PrepareOutput() != 0 ||
        ProceedOutput() != 0)
    {
        sLog->Error("Fatal error occurred during execution, exiting");
        return 1;
    }

    // TODO: cleanup, finalize, print statistics?

    sLog->Info("All procedures finished without error");

    return 0;
}
