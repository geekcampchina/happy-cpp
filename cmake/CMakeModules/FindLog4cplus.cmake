#.rst:
# FindLog4cplus
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    LOG4CPLUS_FOUND - Found the log4cplus
#    LOG4CPLUS_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    LOG4CPLUS_BOTH_LIBRARIES - Both liblog4cplus
#    LOG4CPLUS_LIBRARIES - liblog4cplus
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    LOG4CPLUS_ROOT - (as a CMake or environment variable)
#                 The root directory of the log4cplus install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Log4cplus REQUIRED)
#     include_directories(${LOG4CPLUS_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.c)
#     target_link_libraries(foo ${LOG4CPLUS_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_log4cplus_append_debugs _endvar _library)
    if (${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else ()
        set(_output ${${_library}})
    endif ()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_log4cplus_find_library _name)
    find_library(${_name}
            NAMES ${ARGN}
            HINTS
            ENV LOG4CPLUS_ROOT
            ${LOG4CPLUS_ROOT}
            PATH_SUFFIXES ${_log4cplus_libpath_suffixes}
            )
    mark_as_advanced(${_name})
endfunction()


set(_log4cplus_libpath_suffixes lib)

find_path(LOG4CPLUS_INCLUDE_DIR
        NAMES log4cplus/log4cplus.h
        HINTS $ENV{LOG4CPLUS_ROOT} ${LOG4CPLUS_ROOT}
        PATH_SUFFIXES include
        )
mark_as_advanced(LOG4CPLUS_INCLUDE_DIR)

_log4cplus_find_library(LOG4CPLUS_LIBRARY log4cplus)
_log4cplus_find_library(LOG4CPLUS_LIBRARY_DEBUG log4cplusD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Log4cplus DEFAULT_MSG LOG4CPLUS_LIBRARY LOG4CPLUS_INCLUDE_DIR)

if (LOG4CPLUS_FOUND)
    set(LOG4CPLUS_INCLUDE_DIRS ${LOG4CPLUS_INCLUDE_DIR})
    _log4cplus_append_debugs(LOG4CPLUS_LIBRARIES LOG4CPLUS_LIBRARY)
    _log4cplus_append_debugs(LOG4CPLUS_PARTED_LIBRARIES LOG4CPLUS_PARTED_LIBRARY)
    set(LOG4CPLUS_BOTH_LIBRARIES ${LOG4CPLUS_LIBRARIES} ${LOG4CPLUS_PARTED_LIBRARIES})
endif ()
