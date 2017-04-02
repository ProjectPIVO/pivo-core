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

#ifndef PIVO_OUTPUT_MODULE_FEATURES_H
#define PIVO_OUTPUT_MODULE_FEATURES_H

enum OutputModuleFeatures
{
    OMF_FLAT_PROFILE = 0,
    OMF_CALL_GRAPH = 1,
    OMF_CALL_TREE = 2,
    OMF_HEAT_MAP = 3,
    MAX_OMF
};

#define OMF_SET uint64_t
#define OMF_CREATE(a) a = 0
#define OMF_ADD(a,f) a = a | (1 << f)
#define OMF_REM(a,f) a = a & ~(1 << f)
#define OMF_ISSET(a,f) ((a & (1 << f)) != 0)

#endif
