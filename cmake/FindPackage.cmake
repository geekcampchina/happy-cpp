LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/CMakeModules/")

IF(MSVC)
  # 解决 GTest 编译报错：“std::tuple”: 模板 参数太多
  ADD_DEFINITIONS("-D_VARIADIC_MAX=10")
ENDIF()

FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

IF(MSVC)
  SET(Boost_USE_STATIC_LIBS ON)
  SET(Boost_USE_STATIC_RUNTIME ON)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_DEBUG_RUNTIME ON)
  FIND_PACKAGE(Intl REQUIRED)
  INCLUDE_DIRECTORIES(${INTL_INCLUDE_DIR})
ENDIF()

FIND_PACKAGE(Boost 1.60.0 REQUIRED COMPONENTS filesystem system)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(Pugixml REQUIRED)
INCLUDE_DIRECTORIES(${PUGIXML_INCLUDE_DIRS})

FIND_PACKAGE(Curl REQUIRED)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})

FIND_PACKAGE(OpenSSL REQUIRED)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

FIND_PACKAGE(Log4cplus REQUIRED)
INCLUDE_DIRECTORIES(${LOG4CPLUS_INCLUDE_DIRS})