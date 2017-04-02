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

#ifndef PIVO_HEATMAP_STRUCTS_H
#define PIVO_HEATMAP_STRUCTS_H

struct VolumeHistogramEntry
{
    // total time spent in this function, exclusive only
    double timeTotal;
    // total inclusive time spent in this function
    double timeTotalInclusive;
};

// primary key = function ID, value = structure of histogram data
typedef std::map<uint32_t, VolumeHistogramEntry> VolumeHistogramMap;

// key = time segment ID, value = histogram for that time segment
typedef std::vector<VolumeHistogramMap> TimeHistogramVector;

#endif
