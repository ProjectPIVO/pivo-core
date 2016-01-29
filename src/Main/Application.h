#ifndef PIVO_APPLICATION_H
#define PIVO_APPLICATION_H

#include "Singleton.h"

class Application
{
    friend class Singleton<Application>;
    public:
        ~Application();

        // Initialize application, load command line options, prepare environment
        bool Init(int argc, char** argv);
        // Run main application routine, select module, load, pass arguments, process
        int Run();

    protected:
        // protected singleton constructor
        Application();

    private:
        //
};

#define sApplication Singleton<Application>::instance()

#endif
