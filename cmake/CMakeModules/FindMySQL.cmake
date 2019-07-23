#.rst:
# FindMySQL
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    MYSQL_FOUND - Found mysqlclient
#    MYSQL_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    MYSQL_BOTH_LIBRARIES - Both mysqlclient
#    MYSQL_LIBRARIES - mysqlclient
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    MYSQL_ROOT - (as a CMake or environment variable)
#                 The root directory of the mysql install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(MySQL REQUIRED)
#     include_directories(${MYSQL_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${MYSQL_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_mysql_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_mysql_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV MYSQL_ROOT
            ${MYSQL_ROOT}
        PATH_SUFFIXES ${_mysql_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_mysql_libpath_suffixes lib lib64 lib64/mysql lib/mysql)

find_path(MYSQL_INCLUDE_DIR 
    NAMES mysql/mysql.h
    HINTS $ENV{MYSQL_ROOT} ${MYSQL_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(MYSQL_INCLUDE_DIR)

_mysql_find_library(MYSQL_LIBRARY          mysqlclient)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MySQL DEFAULT_MSG MYSQL_LIBRARY MYSQL_INCLUDE_DIR)

if(MYSQL_FOUND)
    set(MYSQL_INCLUDE_DIRS ${MYSQL_INCLUDE_DIR})
    _mysql_append_debugs(MYSQL_LIBRARIES      MYSQL_LIBRARY)
    set(MYSQL_BOTH_LIBRARIES ${MYSQL_LIBRARIES})
endif()

