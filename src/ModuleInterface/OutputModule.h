#ifndef PIVO_OUTPUT_MODULE_H
#define PIVO_OUTPUT_MODULE_H

#include <vector>

#include "UnitIdentifiers.h"
#include "FlatProfileStructs.h"
#include "OutputModuleFeatures.h"
#include "NormalizedData.h"

class OutputModule
{
    public:
        // Reports module features
        virtual void ReportFeatures(OMF_SET &set) = 0;

        // Performs data visualization as implemented by the module
        virtual void VisualizeData(NormalizedData* data) = 0;

    protected:
        //

    private:
        //
};

#endif
