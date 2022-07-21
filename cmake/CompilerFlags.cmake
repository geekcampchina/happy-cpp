IF (MSVC)
    SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2")
    #/Z7 将pdb合并到库文件 /Zi 生成单独的pdb文件
    SET(CMAKE_CXX_FLAGS_DEBUG "/MTd /Z7 /Ob0 /Od")
    SET(CMAKE_CXX_FLAGS "/EHsc /wd\"4819\" /wd\"4996\"")

    #curl libeay32 ssleay32依赖的宏定义,如果缺少该宏定义,链接库时会出现类似错误:
    #error LNK2019: 无法解析的外部符号 __imp__curl_global_init
    ADD_DEFINITIONS(-DUSE_SSLEAY -DUSE_OPENSSL -DBUILDING_LIBCURL)
ENDIF ()


IF (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "-fPIC -Wall -Wno-deprecated-declarations")
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
ENDIF ()

IF (CMAKE_COMPILER_IS_GNUCXX)
    IF (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.9.0
            OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9.0)
        ADD_DEFINITIONS(-D_USE_CXX_REGEX_)
    ELSE ()
        ADD_DEFINITIONS(-D_USE_BOOST_REGEX_)
    ENDIF ()
ENDIF ()

IF (MSVC)
    ADD_DEFINITIONS(-D_USE_CXX_REGEX_)
ENDIF ()
