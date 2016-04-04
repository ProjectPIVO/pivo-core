#ifndef PIVO_LOG_H
#define PIVO_LOG_H

#include "Singleton.h"

#include <mutex>
#include <thread>

enum LogLevel
{
    LOG_OFF = 0,
    LOG_ERROR = 1,
    LOG_WARNING = 2,
    LOG_INFO = 3,
    LOG_VERBOSE = 4,
    LOG_DEBUG = 5,

    MAX_LOG_LEVEL
};

static const char logLevelChars[MAX_LOG_LEVEL] = {
    '0',    // off, not used at all
    'E',    // LOG_ERROR
    'W',    // LOG_WARNING
    'I',    // LOG_INFO
    'V',    // LOG_VERBOSE
    'D'     // LOG_DEBUG
};

void DllLogFunc(int level, const char* str, ...);

class Log
{
    friend class Singleton<Log>;
    public:
        ~Log();

        // Sets console silence state
        void SetSilent(bool state);
        // Is console output disabled?
        bool IsSilent();

        // Sets log level
        void SetLogLevel(int level);
        // Retrieves log level
        int GetLogLevel();

        // Sets log file and opens stream; returns success/failure status
        bool SetLogFile(const char* filename);
        // Retrieves log filename
        const char* GetLogFile();

        // Error log
        void Error(const char *str, ...);
        // Warning log
        void Warning(const char *str, ...);
        // Info log
        void Info(const char *str, ...);
        // Verbose log
        void Verbose(const char *str, ...);
        // Debug log
        void Debug(const char *str, ...);

        // internal logging method
        void PerformLog(LogLevel level, const char* str);

    protected:
        // protected singleton constructor
        Log();

    private:
        // is console output disabled?
        bool m_isSilent;
        // log level
        int m_logLevel;
        // file output
        FILE* m_outFile;
        // filename of file output
        std::string m_logFileName;

        // logging mutex, for future possibility of using more threads
        std::mutex m_logMutex;
};

#define sLog Singleton<Log>::instance()

#endif
