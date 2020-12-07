#.rst:
# FindRapidJSON
# ---------
#
#
# Defines the following variables:
#
# ::
#
#    RAPIDJSON_FOUND - Found the RapidJSON
#    RAPIDJSON_INCLUDE_DIRS - Include directories
#
#
# Accepts the following variables as input:
#
# ::
#
#    RAPIDJSON_ROOT - (as a CMake or environment variable)
#                 The root directory of the RapidJSON install prefix
#
#
#
# Example Usage:
#
# ::
#
#     enable_testing()
#     find_package(RapidJSON REQUIRED)
#     include_directories(${RAPIDJSON_INCLUDE_DIRS})
#

find_path(RAPIDJSON_INCLUDE_DIR
        NAMES rapidjson/rapidjson.h rapidjson/document.h
        HINTS $ENV{RAPIDJSON_ROOT} ${RAPIDJSON_ROOT}
        PATH_SUFFIXES include
        )
mark_as_advanced(RAPIDJSON_INCLUDE_DIR)


include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RapidJSON DEFAULT_MSG RAPIDJSON_INCLUDE_DIR)

if (RAPIDJSON_FOUND)
    set(RAPIDJSON_INCLUDE_DIRS ${RAPIDJSON_INCLUDE_DIR})
endif ()

