#.rst:
# FindCurl
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    CURL_FOUND - Found the curl
#    CURL_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    CURL_BOTH_LIBRARIES - Both libcurl
#    CURL_LIBRARIES - libcurl
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    CURL_ROOT - (as a CMake or environment variable)
#                 The root directory of the curl install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Curl REQUIRED)
#     include_directories(${CURL_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${CURL_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_curl_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_curl_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV CURL_ROOT
            ${CURL_ROOT}
        PATH_SUFFIXES ${_curl_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_curl_libpath_suffixes lib)

find_path(CURL_INCLUDE_DIR 
    NAMES curl/curl.h
    HINTS $ENV{CURL_ROOT} ${CURL_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(CURL_INCLUDE_DIR)

_curl_find_library(CURL_LIBRARY          curl)
_curl_find_library(CURL_LIBRARY_DEBUG    curld)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Curl DEFAULT_MSG CURL_LIBRARY CURL_INCLUDE_DIR)

if(CURL_FOUND)
    set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
    _curl_append_debugs(CURL_LIBRARIES      CURL_LIBRARY)
    set(CURL_BOTH_LIBRARIES ${CURL_LIBRARIES})
endif()

