#.rst:
# FindHiredis
# ---------
#
# Defines the following variables:
#
# ::
#
#    HIREDIS_FOUND - Found the hiredis
#    HIREDIS_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    HIREDIS_BOTH_LIBRARIES - Both libhiredis
#    HIREDIS_LIBRARIES - libhiredis
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    HIREDIS_ROOT - (as a CMake or environment variable)
#                 The root directory of the hiredis install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Hiredis REQUIRED)
#     include_directories(${HIREDIS_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${HIREDIS_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_hiredis_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_hiredis_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV HIREDIS_ROOT
            ${HIREDIS_ROOT}
        PATH_SUFFIXES ${_hiredis_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_hiredis_libpath_suffixes lib)

find_path(HIREDIS_INCLUDE_DIR 
    NAMES hiredis/hiredis.h
    HINTS $ENV{HIREDIS_ROOT} ${HIREDIS_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(HIREDIS_INCLUDE_DIR)

_hiredis_find_library(HIREDIS_LIBRARY          hiredis)
_hiredis_find_library(HIREDIS_LIBRARY_DEBUG    hiredisD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Hiredis DEFAULT_MSG HIREDIS_LIBRARY HIREDIS_INCLUDE_DIR)

if(HIREDIS_FOUND)
    set(HIREDIS_INCLUDE_DIRS ${HIREDIS_INCLUDE_DIR})
    _hiredis_append_debugs(HIREDIS_LIBRARIES      HIREDIS_LIBRARY)
    set(HIREDIS_BOTH_LIBRARIES ${HIREDIS_LIBRARIES})
endif()

