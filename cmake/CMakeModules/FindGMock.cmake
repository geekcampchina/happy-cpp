#.rst:
# FindGMock
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    GMOCK_FOUND - Found the gmock
#    GMOCK_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    GMOCK_BOTH_LIBRARIES - Both libgmock
#    GMOCK_LIBRARIES - libgmock
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    GMOCK_ROOT - (as a CMake or environment variable)
#                 The root directory of the gmock install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(GMock REQUIRED)
#     include_directories(${GMOCK_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${GMOCK_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_gmock_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_gmock_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV GMOCK_ROOT
            ${GMOCK_ROOT}
        PATH_SUFFIXES ${_gmock_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_gmock_libpath_suffixes lib)

find_path(GMOCK_INCLUDE_DIR 
    NAMES gmock/gmock.h
    HINTS $ENV{GMOCK_ROOT} ${GMOCK_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(GMOCK_INCLUDE_DIR)

_gmock_find_library(GMOCK_LIBRARY          gmock)
_gmock_find_library(GMOCK_LIBRARY_DEBUG    gmockD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GMock DEFAULT_MSG GMOCK_LIBRARY GMOCK_INCLUDE_DIR)

if(GMOCK_FOUND)
    set(GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIR})
    _gmock_append_debugs(GMOCK_LIBRARIES      GMOCK_LIBRARY)
    set(GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARIES})
endif()

