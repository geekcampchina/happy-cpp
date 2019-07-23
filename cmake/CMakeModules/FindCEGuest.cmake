#.rst:
# FindCEGuest
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    CEGUEST_FOUND - Found the libceguest
#    CEGUEST_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    CEGUEST_BOTH_LIBRARIES - Both libceguest
#    CEGUEST_LIBRARIES - libceguest
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    CEGUEST_ROOT - (as a CMake or environment variable)
#                 The root directory of the ceguest install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(CEGuest REQUIRED)
#     include_directories(${CEGUEST_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${CEGUEST_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_ceguest_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_ceguest_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV CEGUEST_ROOT
            ${CEGUEST_ROOT}
        PATH_SUFFIXES ${_ceguest_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_ceguest_libpath_suffixes lib)

find_path(CEGUEST_INCLUDE_DIR 
    NAMES ceguest/ceguest_config.h
    HINTS $ENV{CEGUEST_ROOT} ${CEGUEST_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(CEGUEST_INCLUDE_DIR)

_ceguest_find_library(CEGUEST_LIBRARY          ceguest)
_ceguest_find_library(CEGUEST_LIBRARY_DEBUG    ceguestD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEGuest DEFAULT_MSG CEGUEST_LIBRARY CEGUEST_INCLUDE_DIR)

if(CEGUEST_FOUND)
    set(CEGUEST_INCLUDE_DIRS ${CEGUEST_INCLUDE_DIR})
    _ceguest_append_debugs(CEGUEST_LIBRARIES      CEGUEST_LIBRARY)
    set(CEGUEST_BOTH_LIBRARIES ${CEGUEST_LIBRARIES})
endif()

