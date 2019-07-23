#.rst:
# FindCEng
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    CENG_FOUND - Found the libceng
#    CENG_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    CENG_BOTH_LIBRARIES - Both libceng
#    CENG_LIBRARIES - libceng
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    CENG_ROOT - (as a CMake or environment variable)
#                 The root directory of the ceng install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(CEng REQUIRED)
#     include_directories(${CENG_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${CENG_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_ceng_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_ceng_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV CENG_ROOT
            ${CENG_ROOT}
        PATH_SUFFIXES ${_ceng_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_ceng_libpath_suffixes lib)

find_path(CENG_INCLUDE_DIR 
    NAMES ceng/ceng.h
    HINTS $ENV{CENG_ROOT} ${CENG_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(CENG_INCLUDE_DIR)

_ceng_find_library(CENG_LIBRARY          ceng)
_ceng_find_library(CENG_LIBRARY_DEBUG    cengD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CEng DEFAULT_MSG CENG_LIBRARY CENG_INCLUDE_DIR)

if(CENG_FOUND)
    set(CENG_INCLUDE_DIRS ${CENG_INCLUDE_DIR})
    _ceng_append_debugs(CENG_LIBRARIES      CENG_LIBRARY)
    set(CENG_BOTH_LIBRARIES ${CENG_LIBRARIES})
endif()

