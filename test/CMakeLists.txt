FUNCTION(ADD_UNITTEST unittest_name src_files)
    ADD_EXECUTABLE(${unittest_name} ${src_files})
    TARGET_LINK_LIBRARIES(${unittest_name} ${GTEST_BOTH_LIBRARIES}
            happycpp ${DEP_LIBS})

    ADD_TEST(NAME ${unittest_name} COMMAND ${unittest_name})
ENDFUNCTION(ADD_UNITTEST)

ADD_DEFINITIONS(-D_BINARY_DIR_="${CMAKE_BINARY_DIR}")

ADD_UNITTEST(array_unittest algorithm/array_unittest.cc)
ADD_UNITTEST(domain_unittest algorithm/domain_unittest.cc)
ADD_UNITTEST(double_unittest algorithm/double_unittest.cc)
ADD_UNITTEST(int_unittest algorithm/int_unittest.cc)
ADD_UNITTEST(map_unittest algorithm/map_unittest.cc)
ADD_UNITTEST(ip_unittest algorithm/ip_unittest.cc)
ADD_UNITTEST(random_unittest algorithm/random_unittest.cc)
ADD_UNITTEST(string_unittest algorithm/string_unittest.cc)
ADD_UNITTEST(time_unittest algorithm/time_unittest.cc)
ADD_UNITTEST(unit_unittest algorithm/unit_unittest.cc)
ADD_UNITTEST(version_unittest algorithm/version_unittest.cc)
ADD_UNITTEST(byte_unittest algorithm/byte_unittest.cc)
ADD_UNITTEST(enum_unittest algorithm/enum_unittest.cc)
ADD_UNITTEST(cmd_unittest cmd_unittest.cc)
ADD_UNITTEST(errno_unittest errno_unittest.cc)
ADD_UNITTEST(exception_unittest exception_unittest.cc)
ADD_UNITTEST(filesys_unittest filesys_unittest.cc)
ADD_UNITTEST(http_unittest http_unittest.cc)
ADD_UNITTEST(i18n_unittest i18n_unittest.cc)
ADD_UNITTEST(os_unittest os_unittest.cc)
ADD_UNITTEST(proc_unittest proc_unittest.cc)
ADD_UNITTEST(xml_unittest xml_unittest.cc)
ADD_UNITTEST(happycpp_unittest happycpp_unittest.cc)
ADD_UNITTEST(iconv_unittest iconv_unittest.cc)
