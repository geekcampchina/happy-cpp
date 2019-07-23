#.rst:
# FindEasyOpenSSL
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    EASYOPENSSL_FOUND - Found the easyopenssl
#    EASYOPENSSL_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    EASYOPENSSL_BOTH_LIBRARIES - Both libeasyopenssl
#    EASYOPENSSL_LIBRARIES - libeasyopenssl
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    EASYOPENSSL_ROOT - (as a CMake or environment variable)
#                 The root directory of the easyopenssl install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(EasyOpenSSL REQUIRED)
#     include_directories(${EASYOPENSSL_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${EASYOPENSSL_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_easyopenssl_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_easyopenssl_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV EASYOPENSSL_ROOT
            ${EASYOPENSSL_ROOT}
        PATH_SUFFIXES ${_easyopenssl_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_easyopenssl_libpath_suffixes lib)

find_path(EASYOPENSSL_INCLUDE_DIR 
    NAMES easyopenssl/config.hxx easyopenssl/appender.h easyopenssl/loglevel.h
    HINTS $ENV{EASYOPENSSL_ROOT} ${EASYOPENSSL_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(EASYOPENSSL_INCLUDE_DIR)

_easyopenssl_find_library(EASYOPENSSL_LIBRARY          easyopenssl)
_easyopenssl_find_library(EASYOPENSSL_LIBRARY_DEBUG    easyopensslD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(EasyOpenSSL DEFAULT_MSG EASYOPENSSL_LIBRARY EASYOPENSSL_INCLUDE_DIR)

if(EASYOPENSSL_FOUND)
    set(EASYOPENSSL_INCLUDE_DIRS ${EASYOPENSSL_INCLUDE_DIR})
    _easyopenssl_append_debugs(EASYOPENSSL_LIBRARIES      EASYOPENSSL_LIBRARY)
    set(EASYOPENSSL_BOTH_LIBRARIES ${EASYOPENSSL_LIBRARIES})
endif()

