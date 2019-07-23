#.rst:
# FindVhm
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    VHM_FOUND - Found the vhm
#    VHM_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    VHM_BOTH_LIBRARIES - Both libvhm
#    VHM_LIBRARIES - libvhm
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    VHM_ROOT - (as a CMake or environment variable)
#                 The root directory of the vhm install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Vhm REQUIRED)
#     include_directories(${VHM_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${VHM_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_vhm_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_vhm_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV VHM_ROOT
            ${VHM_ROOT}
        PATH_SUFFIXES ${_vhm_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_vhm_libpath_suffixes lib)

find_path(VHM_INCLUDE_DIR 
    NAMES vhm/vhm.h
    HINTS $ENV{VHM_ROOT} ${VHM_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(VHM_INCLUDE_DIR)

_vhm_find_library(VHM_LIBRARY          vhm)
_vhm_find_library(VHM_LIBRARY_DEBUG    vhmD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vhm DEFAULT_MSG VHM_LIBRARY VHM_INCLUDE_DIR)

if(VHM_FOUND)
    set(VHM_INCLUDE_DIRS ${VHM_INCLUDE_DIR})
    _vhm_append_debugs(VHM_LIBRARIES      VHM_LIBRARY)
    set(VHM_BOTH_LIBRARIES ${VHM_LIBRARIES})
endif()

