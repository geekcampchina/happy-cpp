CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(happy-cpp CXX)

IF (MSVC)
    SET(BUILD_MODE STATIC)
ELSE ()
    SET(BUILD_MODE SHARED)
ENDIF ()

# 设置默认 CMAKE_BUILD_TYPE 为 release
IF (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release
            CACHE STRING
            "Choose the type of build, options are: Debug Release \
          RelWithDebInfo MinSizeRel."
            FORCE)
ENDIF (NOT CMAKE_BUILD_TYPE)

INCLUDE(cmake/CompilerChecks.cmake)
INCLUDE(cmake/CompilerFlags.cmake)
INCLUDE(cmake/FindPackage.cmake)
INCLUDE(cmake/OutputDir.cmake)

INCLUDE_DIRECTORIES(include)

OPTION(BUILD_TESTING "构建测试用例。" ON)

IF (MSVC)
    SET(HAPPYCPP_SHAREDLIB OFF CACHE BOOL
            "如果值为 ON，HAPPYCPP 将构建为共享库, 反之为静态库。")
ELSE ()
    SET(HAPPYCPP_SHAREDLIB ON CACHE BOOL
            "如果值为 ON，HAPPYCPP 将构建为共享库, 反之为静态库。")
ENDIF ()

# 所有使用 happy-cpp 静态库的项目都必须定义 HAPPYCPP_STATICLIB 宏
IF (NOT HAPPYCPP_SHAREDLIB)
    ADD_DEFINITIONS("-DHAPPYCPP_STATICLIB")
ENDIF ()

SET(DEP_LIBS ${Boost_LIBRARIES}
        ${PUGIXML_LIBRARIES}
        ${CURL_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${LOG4CPLUS_LIBRARIES})

IF (MSVC)
    SET(DEP_LIBS ${DEP_LIBS}
            ${INTL_LIBRARIES}
            comsuppw
            iphlpapi
            netapi32
            ws2_32
            wldap32)
ENDIF ()

ENABLE_TESTING()

INSTALL(DIRECTORY include/happycpp DESTINATION include)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
