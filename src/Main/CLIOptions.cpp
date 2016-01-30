#include "General.h"
#include "Application.h"

bool Application::AddDefaultOption(const char* name, CommandLineOption op, CommandLineOptionType type)
{
    if (m_cliOpts.find(op) != m_cliOpts.end())
    {
        // TODO: log message
        return false;
    }

    m_cliOptNames[name] = op;

    m_cliOpts[op] = CommandLineOptionValue();
    m_cliOpts[op].type = type;

    return true;
}

void Application::SetStringOption(CommandLineOption op, const char* value)
{
    int len = strlen(value);

    m_cliOpts[op].value.asString = new char[len + 1];
    strncpy(m_cliOpts[op].value.asString, value, len);
    m_cliOpts[op].value.asString[len] = '\0';
}

void Application::SetIntOption(CommandLineOption op, int64_t value)
{
    m_cliOpts[op].value.asInt = value;
}

void Application::SetBoolOption(CommandLineOption op, bool value)
{
    m_cliOpts[op].value.asInt = value ? 1 : 0;
}

void Application::AddDefaultOptionString(const char* name, CommandLineOption op, const char* value)
{
    if (!AddDefaultOption(name, op, CLIOPTYPE_STRING))
        return;

    SetStringOption(op, value);
}

void Application::AddDefaultOptionInt(const char* name, CommandLineOption op, int64_t value)
{
    if (!AddDefaultOption(name, op, CLIOPTYPE_INT))
        return;

    SetIntOption(op, value);
}

void Application::AddDefaultOptionBool(const char* name, CommandLineOption op, bool value)
{
    if (!AddDefaultOption(name, op, CLIOPTYPE_BOOL))
        return;

    SetBoolOption(op, value);
}

void Application::AddOptionNameAlias(const char* name, CommandLineOption op)
{
    m_cliOptNames[name] = op;
}

CommandLineOption Application::GetCLIOptionByName(const char* name)
{
    if (m_cliOptNames.find(name) == m_cliOptNames.end())
    {
        // TODO: log message
        return MAX_CLIOPT;
    }

    return m_cliOptNames[name];
}

CommandLineOptionUnion* Application::GetCLIOption(CommandLineOption op, CommandLineOptionType type)
{
    if (m_cliOpts.find(op) == m_cliOpts.end())
    {
        // TODO: log message
        return nullptr;
    }

    CommandLineOptionValue &opt = m_cliOpts[op];

    if (opt.type != type)
    {
        // TODO: log message
        return nullptr;
    }

    return &opt.value;
}

std::string Application::GetStringOption(CommandLineOption op)
{
    if (CommandLineOptionUnion* cliun = GetCLIOption(op, CLIOPTYPE_STRING))
        return std::string(cliun->asString);

    return UNDEFINED_STR;
}

int64_t Application::GetIntOption(CommandLineOption op)
{
    if (CommandLineOptionUnion* cliun = GetCLIOption(op, CLIOPTYPE_INT))
        return cliun->asInt;

    return UNDEFINED_INT;
}

bool Application::GetBoolOption(CommandLineOption op)
{
    if (CommandLineOptionUnion* cliun = GetCLIOption(op, CLIOPTYPE_BOOL))
        return (cliun->asInt != 0);

    return false;
}

bool Application::IsOptionSet(CommandLineOption op)
{
    if (m_cliOpts.find(op) == m_cliOpts.end())
        return false;

    CommandLineOptionValue &val = m_cliOpts[op];

    if (val.type == CLIOPTYPE_INT && val.value.asInt == UNDEFINED_INT)
        return false;
    else if (val.type == CLIOPTYPE_STRING && strcmp(val.value.asString, UNDEFINED_STR) == 0)
        return false;

    return true;
}

bool Application::ParseCommandLineOpts(int argc, char** argv)
{
    int i;
    CommandLineOption op;

    // go through all args except first (that's executable name/path)
    for (i = 1; i < argc; i++)
    {
        // retrieve option name
        op = GetCLIOptionByName(argv[i]);
        // if not found, report invalid option value and exit
        if (op == MAX_CLIOPT)
        {
            // TODO: log message
            return false;
        }

        // check presence of this argument in cliopt map
        if (m_cliOpts.find(op) == m_cliOpts.end())
        {
            // TODO: assertion? this should never happen
            return false;
        }

        switch (m_cliOpts[op].type)
        {
            case CLIOPTYPE_BOOL:
            {
                // boolean options sets its option always to true
                SetBoolOption(op, true);
                break;
            }
            case CLIOPTYPE_INT:
            {
                // for this type of parameter there should be value present
                if (argc <= i + 1)
                {
                    // TODO: log message
                    return false;
                }

                char* endptr;
                int64_t lval;

                try
                {
                    lval = std::strtoll(argv[i + 1], &endptr, 10);
                }
                catch (...)
                {
                    lval = 0;
                    endptr = argv[i + 1];
                }

                if (lval == 0 && *endptr != '\0')
                {
                    // TODO: log message
                    return false;
                }

                SetIntOption(op, lval);

                i++;
                break;
            }
            case CLIOPTYPE_STRING:
            {
                // for this type of parameter there should be value present
                if (argc <= i + 1)
                {
                    // TODO: log message
                    return false;
                }

                SetStringOption(op, argv[i + 1]);

                i++;
                break;
            }
            default:
            {
                // this should not happen, but who knows..
                // TODO: log message
                return false;
            }
        }
    }

    return true;
}
