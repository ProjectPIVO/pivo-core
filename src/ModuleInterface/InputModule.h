#ifndef PIVO_INPUT_MODULE_H
#define PIVO_INPUT_MODULE_H

#include <vector>

#include "UnitIdentifiers.h"
#include "InputModuleFeatures.h"
#include "FlatProfileStructs.h"
#include "CallGraphStructs.h"

class InputModule
{
    public:
        // Reports module name
        virtual const char* ReportName() = 0;
        // Reports module version
        virtual const char* ReportVersion() = 0;
        // Reports module features
        virtual void ReportFeatures(IMF_SET &set) = 0;
        // Loads file into memory for future processing, return true on success
        virtual bool LoadFile(const char* file, const char* binaryFile) = 0;

        // Retrieves class table loaded from file
        virtual void GetClassTable(std::vector<ClassEntry> &dst) = 0;
        // Retrieves function table loaded from file
        virtual void GetFunctionTable(std::vector<FunctionEntry> &dst) = 0;

        // Retrieves flat profile data
        virtual void GetFlatProfileData(std::vector<FlatProfileRecord> &dst) = 0;
        // Retrieves call graph map data
        virtual void GetCallGraphMap(CallGraphMap &dst) = 0;

    protected:
        //

    private:
        //
};

#endif
