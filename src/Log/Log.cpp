#include "General.h"
#include "Log.h"

#include <cstdarg>

#define FORMAT_LOG_ARGS(buf, n) \
    va_list argList;\
    va_start(argList, str);\
    vsnprintf(buf, n, str, argList);\
    va_end(argList);

Log::Log() : m_logFileName("")
{
    m_outFile = nullptr;
}

Log::~Log()
{
    if (m_outFile)
        fclose(m_outFile);
}

bool Log::SetLogFile(const char* filename)
{
    FILE* tmp;

    // try to open new file for writing
    tmp = fopen(filename, "w");
    if (!tmp)
        return false;

    std::unique_lock<std::mutex> lck(m_logMutex);

    // on success, close old log file (if any)
    if (m_outFile)
        fclose(m_outFile);

    // set new log file
    m_outFile = tmp;
    m_logFileName = filename;

    return true;
}

const char* Log::GetLogFile()
{
    return m_logFileName.c_str();
}

void Log::SetSilent(bool state)
{
    m_isSilent = state;
}

bool Log::IsSilent()
{
    return m_isSilent;
}

void Log::SetLogLevel(int level)
{
    m_logLevel = level;
}

int Log::GetLogLevel()
{
    return m_logLevel;
}

void Log::PerformLog(LogLevel level, const char* str)
{
    if (m_logLevel < level)
        return;

    std::unique_lock<std::mutex> lck(m_logMutex);

    // if log file is present, log to file also
    if (m_outFile)
    {
        fputs(str, m_outFile);
        fputs("\n", m_outFile);
    }

    // silent = no log output to console
    if (m_isSilent)
        return;

    // output warnings and errors to stderr stream, other logs to stdout
    if (level <= LOG_WARNING)
        std::cerr << str << std::endl;
    else
        std::cout << str << std::endl;
}

void Log::Error(const char *str, ...)
{
    char buf[2048];
    FORMAT_LOG_ARGS(buf, 2048);

    PerformLog(LOG_ERROR, buf);
}

void Log::Warning(const char *str, ...)
{
    char buf[2048];
    FORMAT_LOG_ARGS(buf, 2048);

    PerformLog(LOG_WARNING, buf);
}

void Log::Info(const char *str, ...)
{
    char buf[2048];
    FORMAT_LOG_ARGS(buf, 2048);

    PerformLog(LOG_INFO, buf);
}

void Log::Verbose(const char *str, ...)
{
    char buf[2048];
    FORMAT_LOG_ARGS(buf, 2048);

    PerformLog(LOG_VERBOSE, buf);
}

void Log::Debug(const char *str, ...)
{
    char buf[2048];
    FORMAT_LOG_ARGS(buf, 2048);

    PerformLog(LOG_DEBUG, buf);
}
