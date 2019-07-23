#.rst:
# FindPugixml
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    PUGIXML_FOUND - Found the pugixml
#    PUGIXML_INCLUDE_DIRS - Include directories
#
#
#
# Also defines the library variables below as normal variables.  These
# contain debug/optimized keywords when a debugging library is found.
#
# ::
#
#    PUGIXML_BOTH_LIBRARIES - Both libpugixml
#    PUGIXML_LIBRARIES - libpugixml
#
#
#
# Accepts the following variables as input:
#
# ::
#
#    PUGIXML_ROOT - (as a CMake or environment variable)
#                 The root directory of the pugixml install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(Pugixml REQUIRED)
#     include_directories(${PUGIXML_INCLUDE_DIRS})
#
#
#
# ::
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${PUGIXML_BOTH_LIBRARIES})
#
#
#
# ::
#
#     add_test(AllTestsInFoo foo)
#
#
#

function(_pugixml_append_debugs _endvar _library)
    if(${_library} AND ${_library}_DEBUG)
        set(_output optimized ${${_library}} debug ${${_library}_DEBUG})
    else()
        set(_output ${${_library}})
    endif()
    set(${_endvar} ${_output} PARENT_SCOPE)
endfunction()

function(_pugixml_find_library _name)
    find_library(${_name}
        NAMES ${ARGN}
        HINTS
            ENV PUGIXML_ROOT
            ${PUGIXML_ROOT}
        PATH_SUFFIXES ${_pugixml_libpath_suffixes}
    )
    mark_as_advanced(${_name})
endfunction()



set(_pugixml_libpath_suffixes lib)

find_path(PUGIXML_INCLUDE_DIR 
    NAMES pugixml.hpp
    HINTS $ENV{PUGIXML_ROOT} ${PUGIXML_ROOT}
    PATH_SUFFIXES include
)
mark_as_advanced(PUGIXML_INCLUDE_DIR)

_pugixml_find_library(PUGIXML_LIBRARY          pugixml)
_pugixml_find_library(PUGIXML_LIBRARY_DEBUG    pugixmlD)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pugixml DEFAULT_MSG PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR)

if(PUGIXML_FOUND)
    set(PUGIXML_INCLUDE_DIRS ${PUGIXML_INCLUDE_DIR})
    _pugixml_append_debugs(PUGIXML_LIBRARIES      PUGIXML_LIBRARY)
    set(PUGIXML_BOTH_LIBRARIES ${PUGIXML_LIBRARIES})
endif()

