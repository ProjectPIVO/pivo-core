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

#ifndef PIVO_APPLICATION_H
#define PIVO_APPLICATION_H

#include "Singleton.h"
#include "CLIOptions.h"

#include "UnitIdentifiers.h"
#include "InputModuleFeatures.h"
#include "OutputModuleFeatures.h"
#include "FlatProfileStructs.h"
#include "NormalizedData.h"

enum CommandLineOption
{
    CLIOPT_INPUT_MODULE = 0,
    CLIOPT_OUTPUT_MODULE,
    CLIOPT_LOG_FILE,
    CLIOPT_LOG_LEVEL,
    CLIOPT_INPUT_PATH,
    CLIOPT_OUTPUT_PATH,
    CLIOPT_SILENT,
    CLIOPT_INPUT_BINARY_PATH,
    CLIOPT_PRINTHELP,
    MAX_CLIOPT
};

#ifdef _WIN32
#define SHARED_LIBRARY_SUFFIX ".dll"
#define DLL_INSTANCE HMODULE
#define INPUT_MODULE_NAME_PREFIX "pivo-input-"
#define OUTPUT_MODULE_NAME_PREFIX "pivo-output-"
#else
#define SHARED_LIBRARY_SUFFIX ".so"
#define DLL_INSTANCE void*
#define INPUT_MODULE_NAME_PREFIX "libpivo-input-"
#define OUTPUT_MODULE_NAME_PREFIX "libpivo-output-"
#endif

class InputModule;
class OutputModule;

typedef void(*RegisterLogFunc)(void(*)(int, const char*, ...));
typedef InputModule*(*InputModuleCreateFunc)();
typedef OutputModule*(*OutputModuleCreateFunc)();

class Application
{
    friend class Singleton<Application>;
    public:
        ~Application();

        // Initialize application, load command line options, prepare environment
        bool Init(int argc, char** argv);
        // Run main application routine, select module, load, pass arguments, process
        int Run();

        // Get command line string option
        std::string GetStringOption(CommandLineOption op);
        // Get command line integer option
        int64_t GetIntOption(CommandLineOption op);
        // Get command line boolean option
        bool GetBoolOption(CommandLineOption op);
        // Is command line option set? Applicable only to string and int arguments
        bool IsOptionSet(CommandLineOption op);

    protected:
        // protected singleton constructor
        Application();

        // Adds default option, this method is called inside all other "default option adding" methods
        bool AddDefaultOption(const char* name, CommandLineOption op, CommandLineOptionType type);

        // Retrieves command line option value union
        CommandLineOptionUnion* GetCLIOption(CommandLineOption op, CommandLineOptionType type);

        // Adds default option value (string type)
        void AddDefaultOptionString(const char* name, CommandLineOption op, const char* value);
        // Adds default option value (integer type)
        void AddDefaultOptionInt(const char* name, CommandLineOption op, int64_t value);
        // Adds default option value (boolean type)
        void AddDefaultOptionBool(const char* name, CommandLineOption op, bool value);
        // Adds option name alias
        void AddOptionNameAlias(const char* name, CommandLineOption op);

        // Sets string option value; this method does not verify option type!
        void SetStringOption(CommandLineOption op, const char* value);
        // Sets integer option value; this method does not verify option type!
        void SetIntOption(CommandLineOption op, int64_t value);
        // Sets boolean option value; this method does not verify option type!
        void SetBoolOption(CommandLineOption op, bool value);

        // Retrieves CLI option by name (or alias)
        CommandLineOption GetCLIOptionByName(const char* name);

        // Init defaults, and fill accepted option name map
        void InitCommandLineOpts();

        // Parse command line options, fill maps
        bool ParseCommandLineOpts(int argc, char** argv);

        // Create library handle for input module
        bool CreateInputModuleHandle();
        // Create library handle for output module
        bool CreateOutputModuleHandle();

        // Initializes input module, loads supplied file, etc
        int InitInput();
        // Initializes output module, mainly just checks presence and validity
        int InitOutput();
        // Gathers data from input module - different views, metadata
        int GatherData();
        // Prepares output structures, analyzes gatehered data and creates data for specific views
        int PrepareOutput();
        // Sends processed structures to output module for generating output
        int ProceedOutput();

        // Fills summary block with general information
        void FillSummaryBlock(double totalTime);

    private:
        // map of command line options
        std::map<CommandLineOption, CommandLineOptionValue> m_cliOpts;
        // map of associated strings with command line option enum values
        std::map<std::string, CommandLineOption> m_cliOptNames;

        // vector of all classes present in application
        std::vector<ClassEntry> m_classTable;
        // vector of all functions (methods) present in application
        std::vector<FunctionEntry> m_functionTable;

        // Input module instance
        InputModule* m_inputModule;
        // Input module handle
        DLL_INSTANCE m_inputModuleHandle;
        // Input module reported features
        IMF_SET m_inputModuleFeatures;

        // Input module instance
        OutputModule* m_outputModule;
        // Input module handle
        DLL_INSTANCE m_outputModuleHandle;
        // Input module reported features
        OMF_SET m_outputModuleFeatures;

        // data prepared to be sent to output module
        NormalizedData* m_data;
};

#define sApplication Singleton<Application>::instance()

#endif
