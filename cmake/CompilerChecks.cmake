IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.2)
    MESSAGE(FATAL_ERROR "Insufficient gcc version.")
  ENDIF()
ELSEIF(MSVC)
  # VS2015(19.0.0)
  IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.0)
    MESSAGE(FATAL_ERROR "Insufficient msvc version.")
  ENDIF()
ELSE()
  MESSAGE(FATAL_ERROR
          "You are using an unsupported compiler! Compilation has "
          "only been tested with GCC and MSVC.")
ENDIF()