/**
 * Copyright (C) 2016 Martin Ubl <http://pivo.kennny.cz>
 *
 * This file is part of PIVO suite.
 *
 * PIVO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PIVO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PIVO. If not, see <http://www.gnu.org/licenses/>.
 **/

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
        // Reports module name
        virtual const char* ReportName() = 0;
        // Reports module version
        virtual const char* ReportVersion() = 0;
        // Reports module features
        virtual void ReportFeatures(OMF_SET &set) = 0;

        // Performs data visualization as implemented by the module
        virtual bool VisualizeData(NormalizedData* data) = 0;

    protected:
        //

    private:
        //
};

#endif
