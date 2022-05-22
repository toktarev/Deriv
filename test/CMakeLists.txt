include_directories(/usr/local/include)
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${RAPIDJSON_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include/)
include_directories(${GTEST_INCLUDE_DIR})

subdirs("src")

macro(add_test_file TESTNAME TESTSOURCE)
    add_executable(${TESTNAME} "${TESTSOURCE}")
    add_dependencies(${TESTNAME}  DerivLib gtest)

    set_target_properties(${TESTNAME} PROPERTIES OUTPUT_NAME "${TESTNAME}.test")
    target_link_libraries(${TESTNAME} DerivLib libgtest)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endmacro()

#We should probably add more unit tests
add_test_file(CircularBufferTest "src/circular_buffer_test.cpp")
