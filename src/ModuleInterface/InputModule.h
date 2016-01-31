#ifndef PIVO_INPUT_MODULE_H
#define PIVO_INPUT_MODULE_H

#include "UnitIdentifiers.h"

class InputModule
{
    public:
        // Loads file into memory for future processing, return true on success
        virtual bool LoadFile(const char* file) = 0;

    protected:
        //

    private:
        //
};

#endif
