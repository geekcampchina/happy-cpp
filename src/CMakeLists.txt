IF (MSVC)
    MACRO(GetWin32Winnt version)
        IF (WIN32 AND CMAKE_SYSTEM_VERSION)
            SET(ver ${CMAKE_SYSTEM_VERSION})
            STRING(REPLACE "." "" ver ${ver})
            STRING(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

            SET(${version} "0x${ver}")
        ENDIF ()
    ENDMACRO()

    GetWin32Winnt(ver)
    ADD_DEFINITIONS(-D_WIN32_WINNT=${ver})

    #省略默认库，生成的库文件中不包含libcmt.lib这样的默认库
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zl")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zl")
ENDIF ()

SET(SRC_LIST
        happycpp.cc
        i18n.cc
        xml.cc
        http.cc
        errno.cc
        exception.cc
        algorithm/domain.cc
        algorithm/double.cc
        algorithm/int.cc
        algorithm/ip.cc
        algorithm/random.cc
        algorithm/string.cc
        algorithm/time.cc
        algorithm/unit.cc
        log.cc
        cmd.cc
        filesys.cc
        proc.cc
        os.cc)

IF (MSVC)
    SET(SRC_LIST ${SRC_LIST} windows.cc)

    ADD_LIBRARY(happycpp STATIC ${SRC_LIST})
    TARGET_LINK_LIBRARIES(happycpp ${DEP_LIBS})
ELSE ()
    SET(SRC_LIST ${SRC_LIST} linux.cc)

    ADD_LIBRARY(happycpp SHARED ${SRC_LIST})
    TARGET_LINK_LIBRARIES(happycpp ${DEP_LIBS})
ENDIF ()

INSTALL(TARGETS happycpp DESTINATION lib)