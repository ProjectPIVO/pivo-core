
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
