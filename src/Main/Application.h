#ifndef PIVO_APPLICATION_H
#define PIVO_APPLICATION_H

#include "Singleton.h"
#include "CLIOptions.h"

enum CommandLineOption
{
    CLIOPT_INPUT_MODULE = 0,
    CLIOPT_OUTPUT_MODULE,
    CLIOPT_LOG_FILE,
    CLIOPT_LOG_LEVEL,
    CLIOPT_INPUT_PATH,
    CLIOPT_OUTPUT_PATH,
    CLIOPT_SILENT,
    MAX_CLIOPT
};

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

    private:
        // map of command line options
        std::map<CommandLineOption, CommandLineOptionValue> m_cliOpts;
        // map of associated strings with command line option enum values
        std::map<std::string, CommandLineOption> m_cliOptNames;
};

#define sApplication Singleton<Application>::instance()

#endif
