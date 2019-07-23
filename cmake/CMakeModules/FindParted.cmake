#.rst:
# FindParted
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    PARTED_FOUND - Found the parted
#    PARTED_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    PARTED_BOTH_LIBRARIES - Both libparted
#    PARTED_LIBRARIES - libparted
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    PARTED_ROOT - (as a CMake or environment variable)
#                 The root directory of the parted install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Parted REQUIRED)
#     include_directories(${PARTED_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${PARTED_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_parted_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_parted_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV PARTED_ROOT
            ${PARTED_ROOT}
        PATH_SUFFIXES ${_parted_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_parted_libpath_suffixes lib)

find_path(PARTED_INCLUDE_DIR 
    NAMES parted/parted.h
    HINTS $ENV{PARTED_ROOT} ${PARTED_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(PARTED_INCLUDE_DIR)

_parted_find_library(PARTED_LIBRARY          parted)
_parted_find_library(PARTED_LIBRARY_DEBUG    partedD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Parted DEFAULT_MSG PARTED_LIBRARY PARTED_INCLUDE_DIR)

if(PARTED_FOUND)
    set(PARTED_INCLUDE_DIRS ${PARTED_INCLUDE_DIR})
    _parted_append_debugs(PARTED_LIBRARIES      PARTED_LIBRARY)
    set(PARTED_BOTH_LIBRARIES ${PARTED_LIBRARIES})
endif()

