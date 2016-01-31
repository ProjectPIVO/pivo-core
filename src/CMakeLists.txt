
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

# Prepare file list (empty for now)
SET(corefiles )

# Go through all subdirectories
FOREACH(subdir ${SUBDIRS})
    # All of them should also serve as include directories
    INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES}
        ${subdir}
    )

    GET_FILENAME_COMPONENT(abspath ${subdir} REALPATH)

    # Set include directories to global property
	GET_PROPERTY(core_includes GLOBAL PROPERTY core_include_dirs)
    SET_PROPERTY(GLOBAL PROPERTY core_include_dirs ${core_includes} ${abspath})

    # Search for all source and header files, and append them
    FILE(GLOB tmp_src
        ${subdir}/*.h
        ${subdir}/*.cpp
        ${subdir}/*.c
    )

    # Create filter (MS Visual Studio) for every subdirectory
    SOURCE_GROUP(${subdir} FILES ${tmp_src})

    # Append current source list to all file list
    SET(corefiles
        ${corefiles}
        ${tmp_src}
    )

    # Report this subdirectory
    MESSAGE(STATUS "Added source directory " ${subdir})
ENDFOREACH()

# core part is also executable - add executable to be built from these files
ADD_EXECUTABLE (pivo-core ${corefiles})
