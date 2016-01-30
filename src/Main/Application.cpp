#include "General.h"
#include "Application.h"

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
    // and set them default values, optinally aliases

    AddDefaultOptionString("--input-module", CLIOPT_INPUT_MODULE, UNDEFINED_STR);
    AddOptionNameAlias("-im", CLIOPT_INPUT_MODULE);

    AddDefaultOptionString("--output-module", CLIOPT_OUTPUT_MODULE, UNDEFINED_STR);
    AddOptionNameAlias("-om", CLIOPT_OUTPUT_MODULE);

    AddDefaultOptionString("--log-file", CLIOPT_LOG_FILE, UNDEFINED_STR);
    AddOptionNameAlias("-lf", CLIOPT_LOG_FILE);

    // TODO: implement log, define appropriate levels
    AddDefaultOptionInt("--log-level", CLIOPT_LOG_LEVEL, 0);
    AddOptionNameAlias("-ll", CLIOPT_LOG_LEVEL);

    AddDefaultOptionString("--input", CLIOPT_INPUT_PATH, UNDEFINED_STR);
    AddOptionNameAlias("-i", CLIOPT_INPUT_PATH);

    AddDefaultOptionString("--output", CLIOPT_OUTPUT_PATH, UNDEFINED_STR);
    AddOptionNameAlias("-o", CLIOPT_OUTPUT_PATH);

    AddDefaultOptionBool("--silent", CLIOPT_SILENT, false);
    AddOptionNameAlias("-s", CLIOPT_SILENT);
}

bool Application::Init(int argc, char** argv)
{
    // TODO: init log, at first, output everything to console

    InitCommandLineOpts();

    if (!ParseCommandLineOpts(argc, argv))
        return false;

    // TODO: init log file, redirect log output to file

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
