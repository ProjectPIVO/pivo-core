#ifndef PIVO_INPUT_MODULE_H
#define PIVO_INPUT_MODULE_H

#include <vector>

#include "UnitIdentifiers.h"
#include "InputModuleFeatures.h"

class InputModule
{
    public:
        // Reports module features
        virtual void ReportFeatures(IMF_SET &set) = 0;
        // Loads file into memory for future processing, return true on success
        virtual bool LoadFile(const char* file) = 0;

        // Retrieves class table loaded from file
        virtual void GetClassTable(std::vector<ClassEntry> &dst) = 0;
        // Retrieves function table loaded from file
        virtual void GetFunctionTable(std::vector<FunctionEntry> &dst) = 0;

    protected:
        //

    private:
        //
};

#endif
