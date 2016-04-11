# Copyright (C) 2016 Martin Ubl <http://pivo.kennny.cz>
#
# This file is part of PIVO suite.
#
# PIVO is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# PIVO is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with PIVO. If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(PIVO)

# Use C++11 standard for building
IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(--std=c++11 -g)
ENDIF()

# Turn off CRT secure warnings
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)

# Output directory - binary files (executables, libraries, ..) will be placed here
SET(output_dir "bin")

# Globally set output directory for single-configuration environments (GCC, ..)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${output_dir}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${output_dir}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${output_dir}")

# Globally set output directory for multi-configuration environments (MS Visual Studio, ..)
FOREACH(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    SET(ORIG_OUT_CONFIG ${OUTPUTCONFIG})
    STRING(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${output_dir}/${ORIG_OUT_CONFIG}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${output_dir}/${ORIG_OUT_CONFIG}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${output_dir}/${ORIG_OUT_CONFIG}")
ENDFOREACH(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

# Add main source directory
ADD_SUBDIRECTORY(src)
# Add modules
ADD_SUBDIRECTORY(modules)
