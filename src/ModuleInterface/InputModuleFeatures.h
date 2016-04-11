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

#ifndef PIVO_INPUT_MODULE_FEATURES_H
#define PIVO_INPUT_MODULE_FEATURES_H

enum InputModuleFeatures
{
    IMF_FLAT_PROFILE = 0,
    IMF_CALL_GRAPH = 1,
    IMF_INCLUSIVE_TIME = 2,
    IMF_CALL_TREE = 3,
    IMF_USE_SECONDS = 4,        // use "seconds" as 'time' unit instead of samples
    MAX_IMF
};

#define IMF_SET uint64_t
#define IMF_CREATE(a) a = 0
#define IMF_ADD(a,f) a = a | (1 << f)
#define IMF_REM(a,f) a = a & ~(1 << f)
#define IMF_ISSET(a,f) ((a & (1 << f)) != 0)

#endif
