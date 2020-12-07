#.rst:
# FindSQLapi
# ---------
#
# SQLAPI++ is a C++ library for accessing multiple SQL databases (Oracle, SQL Server, DB2, Sybase, Informix, InterBase, SQLBase, MySQL, PostgreSQL, SQLite, SQL Anywhere and ODBC). 
#
# Defines the following variables:
#
# ::
#
#    SQLAPI_FOUND - Found the SQLapi
#    SQLAPI_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    SQLAPI_BOTH_LIBRARIES - Both libSQLapi
#    SQLAPI_LIBRARIES - libSQLapi
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    SQLAPI_ROOT - (as a CMake or environment variable)
#                 The root directory of the SQLapi install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(SQLapi REQUIRED)
#     include_directories(${SQLAPI_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${SQLAPI_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_SQLapi_append_debugs _endvar _library)
    if (${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else ()
        set(_output ${${_library}})
    endif ()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_SQLapi_find_library _name)
    find_library(${_name}
            NAMES ${ARGN}
            HINTS
            ENV SQLAPI_ROOT
            ${SQLAPI_ROOT}
            PATH_SUFFIXES ${_SQLapi_libpath_suffixes}
            )
    mark_as_advanced(${_name})
endfunction()


set(_SQLapi_libpath_suffixes lib)

find_path(SQLAPI_INCLUDE_DIR
        NAMES sqlapi/SQLAPI.h
        HINTS $ENV{SQLAPI_ROOT} ${SQLAPI_ROOT}
        PATH_SUFFIXES include
        )
mark_as_advanced(SQLAPI_INCLUDE_DIR)

_SQLapi_find_library(SQLAPI_LIBRARY SQLapi)
_SQLapi_find_library(SQLAPI_LIBRARY_DEBUG SQLapiD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SQLapi DEFAULT_MSG SQLAPI_LIBRARY SQLAPI_INCLUDE_DIR)

if (SQLAPI_FOUND)
    set(SQLAPI_INCLUDE_DIRS ${SQLAPI_INCLUDE_DIR})
    _SQLapi_append_debugs(SQLAPI_LIBRARIES SQLAPI_LIBRARY)
    set(SQLAPI_BOTH_LIBRARIES ${SQLAPI_LIBRARIES})
endif ()

