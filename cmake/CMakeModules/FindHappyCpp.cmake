#.rst:
# FindHappyCpp
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    HAPPYCPP_FOUND - Found the happycpp
#    HAPPYCPP_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    HAPPYCPP_BOTH_LIBRARIES - Both libhappycpp
#    HAPPYCPP_LIBRARIES - libhappycpp
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    HAPPYCPP_ROOT - (as a CMake or environment variable)
#                 The root directory of the happycpp install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(HappyCpp REQUIRED)
#     include_directories(${HAPPYCPP_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${HAPPYCPP_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_happycpp_append_debugs _endvar _library)
    if (${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else ()
        set(_output ${${_library}})
    endif ()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_happycpp_find_library _name)
    find_library(${_name}
            NAMES ${ARGN}
            HINTS
            ENV HAPPYCPP_ROOT
            ${HAPPYCPP_ROOT}
            PATH_SUFFIXES ${_happycpp_libpath_suffixes}
            )
    mark_as_advanced(${_name})
endfunction()


set(_happycpp_libpath_suffixes lib)

find_path(HAPPYCPP_INCLUDE_DIR
        NAMES happycpp/happycpp.h
        HINTS $ENV{HAPPYCPP_ROOT} ${HAPPYCPP_ROOT}
        PATH_SUFFIXES include
        )
mark_as_advanced(HAPPYCPP_INCLUDE_DIR)

_happycpp_find_library(HAPPYCPP_LIBRARY happycpp)
_happycpp_find_library(HAPPYCPP_LIBRARY_DEBUG happycppD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(HappyCpp DEFAULT_MSG HAPPYCPP_LIBRARY HAPPYCPP_INCLUDE_DIR)

if (HAPPYCPP_FOUND)
    set(HAPPYCPP_INCLUDE_DIRS ${HAPPYCPP_INCLUDE_DIR})
    _happycpp_append_debugs(HAPPYCPP_LIBRARIES HAPPYCPP_LIBRARY)
    _happycpp_append_debugs(HAPPYCPP_PARTED_LIBRARIES HAPPYCPP_PARTED_LIBRARY)
    set(HAPPYCPP_BOTH_LIBRARIES ${HAPPYCPP_LIBRARIES} ${HAPPYCPP_PARTED_LIBRARIES})
endif ()

