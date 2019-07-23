#.rst:
# FindCEServer
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    CESERVER_FOUND - Found the libceserver
#    CESERVER_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    CESERVER_BOTH_LIBRARIES - Both libceserver
#    CESERVER_LIBRARIES - libceserver
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    CESERVER_ROOT - (as a CMake or environment variable)
#                 The root directory of the ceserver install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(CEServer REQUIRED)
#     include_directories(${CESERVER_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${CESERVER_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_ceserver_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_ceserver_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV CESERVER_ROOT
            ${CESERVER_ROOT}
        PATH_SUFFIXES ${_ceserver_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_ceserver_libpath_suffixes lib)

find_path(CESERVER_INCLUDE_DIR 
    NAMES ceserver/ceserver_config.h
    HINTS $ENV{CESERVER_ROOT} ${CESERVER_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(CESERVER_INCLUDE_DIR)

_ceserver_find_library(CESERVER_LIBRARY          ceserver)
_ceserver_find_library(CESERVER_LIBRARY_DEBUG    ceserverD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEServer DEFAULT_MSG CESERVER_LIBRARY CESERVER_INCLUDE_DIR)

if(CESERVER_FOUND)
    set(CESERVER_INCLUDE_DIRS ${CESERVER_INCLUDE_DIR})
    _ceserver_append_debugs(CESERVER_LIBRARIES      CESERVER_LIBRARY)
    set(CESERVER_BOTH_LIBRARIES ${CESERVER_LIBRARIES})
endif()

