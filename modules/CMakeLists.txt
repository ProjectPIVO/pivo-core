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

# Define macro for selecting "all subdirectories"
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

# Retrieve list of all subdirectories
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Go through all subdirectories
FOREACH(subdir ${SUBDIRS})
    # Include all of them, they will maintain structure creating
    ADD_SUBDIRECTORY(${subdir})

    # Report this subdirectory
    MESSAGE(STATUS "Added module project from directory " ${subdir})
ENDFOREACH()
