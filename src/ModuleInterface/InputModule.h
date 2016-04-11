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

#ifndef PIVO_INPUT_MODULE_H
#define PIVO_INPUT_MODULE_H

#include <vector>

#include "UnitIdentifiers.h"
#include "InputModuleFeatures.h"
#include "FlatProfileStructs.h"
#include "CallGraphStructs.h"
#include "CallTreeStructs.h"

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
        // Retrieves call tree map data
        virtual void GetCallTreeMap(CallTreeMap &dst) = 0;

    protected:
        //

    private:
        //
};

#endif
