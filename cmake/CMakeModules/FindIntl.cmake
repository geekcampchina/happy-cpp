#.rst:
# FindIntl
# ---------
#
# Find the Gettext libintl headers and libraries.
#
# Defines the following variables:
#
# ::
#
#    INTL_FOUND - Found the intl
#    INTL_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    INTL_BOTH_LIBRARIES - Both libintl
#    INTL_LIBRARIES - libintl
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    INTL_ROOT - (as a CMake or environment variable)
#                 The root directory of the intl install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Intl REQUIRED)
#     include_directories(${INTL_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${INTL_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_intl_append_debugs _endvar _library)
    if (${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else ()
        set(_output ${${_library}})
    endif ()

    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_intl_find_library _name)
    find_library(${_name}
            NAMES ${ARGN}
            HINTS
            ENV INTL_ROOT
            ${INTL_ROOT}
            PATH_SUFFIXES ${_intl_libpath_suffixes}
            )
    mark_as_advanced(${_name})
endfunction()


set(_intl_libpath_suffixes lib)

find_path(INTL_INCLUDE_DIR libintl.h
        HINTS $ENV{INTL_ROOT} ${INTL_ROOT}
        PATH_SUFFIXES include
        )
mark_as_advanced(INTL_INCLUDE_DIR)

_intl_find_library(INTL_LIBRARY libintl)
_intl_find_library(INTL_LIBRARY_DEBUG libintlD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Intl DEFAULT_MSG INTL_INCLUDE_DIR)

if (INTL_FOUND)
    set(INTL_INCLUDE_DIRS ${INTL_INCLUDE_DIR})
    _intl_append_debugs(INTL_LIBRARIES INTL_LIBRARY)
    set(INTL_BOTH_LIBRARIES ${INTL_LIBRARIES})
endif ()

