cmake_minimum_required(VERSION 3.15)
project(Deriv)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
set(LOG4CPLUS_VERSION sd_2_0_1)

ExternalProject_Add(
        boost
        DOWNLOAD_COMMAND bash -c "rm -rf ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0 && wget -nc -P ${CMAKE_CURRENT_BINARY_DIR}/download/boost https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.zip && unzip -o ${CMAKE_CURRENT_BINARY_DIR}/download/boost/boost_1_79_0.zip -d ${CMAKE_CURRENT_BINARY_DIR}/external/src"
        PREFIX external
        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/download/boost
        INSTALL_DIR external/out/boost
        CONFIGURE_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0 && ${BOOST_ENV_FLAGS_STR} ./bootstrap.sh ${BOOST_BOOTSTRAP_FLAGS_STR} --prefix=${CMAKE_CURRENT_BINARY_DIR}/external/out/boost --without-icu --with-libraries=filesystem,program_options,system
        BUILD_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0 && ${BOOST_ENV_FLAGS_STR} ./b2 ${BOOST_BUILD_FLAGS_STR} -q -o boost_build.out -d3 -j4 --ignore-site-config threading=multi variant=release link=static --prefix=${CMAKE_CURRENT_BINARY_DIR}/external/out/boost --disable-icu --with-filesystem --with-program_options --with-system
        INSTALL_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/download/boost
        BUILD_IN_SOURCE 1
        LOG_DOWNLOAD ON
)

set(BOOST_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0)

add_library(libboost_asio STATIC IMPORTED)
set(LIBBOOST_ASIO_A ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0/stage/lib/libboost_asio.a)
set_property(TARGET libboost_asio PROPERTY IMPORTED_LOCATION ${LIBBOOST_ASIO_A})

add_library(libboost_system STATIC IMPORTED)
set(LIBBOOST_SYSTEM_A ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0/stage/lib/libboost_system.a)
set_property(TARGET libboost_system PROPERTY IMPORTED_LOCATION ${LIBBOOST_SYSTEM_A})

ExternalProject_Add(
        openssl
        DOWNLOAD_COMMAND bash -c "cd ${CMAKE_CURRENT_BINARY_DIR}/lib && tar -xzf ${CMAKE_CURRENT_BINARY_DIR}/lib/openssl-3.0.3.tar.gz && rm -rf ${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl && mv ${CMAKE_CURRENT_BINARY_DIR}/lib/openssl-3.0.3  ${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl"
        PREFIX external
        INSTALL_DIR external/src/openssl
        CONFIGURE_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl && ./Configure
        BUILD_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl && make
        INSTALL_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl
        BUILD_IN_SOURCE 1
        LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(openssl install_dir)
set(OPENSSL_INCLUDE_DIR ${install_dir}/openssl/include)

ExternalProject_Add(
        websocket
        DOWNLOAD_COMMAND bash -c "unzip -o ${CMAKE_CURRENT_BINARY_DIR}/lib/websocket-0.8.2.zip -d ${CMAKE_CURRENT_BINARY_DIR}/external/src && mv ${CMAKE_CURRENT_BINARY_DIR}/external/src/websocketpp-0.8.2 ${CMAKE_CURRENT_BINARY_DIR}/external/src/websocket"
        PREFIX external
        INSTALL_DIR external/src/websocket
        CONFIGURE_COMMAND cd websocketpp-0.8.2 && cmake .
        BUILD_COMMAND cd websocketpp-0.8.2 && make -j4
        INSTALL_COMMAND cd websocketpp-0.8.2 && make install
        BUILD_IN_SOURCE 1
        LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(websocket install_dir)
set(WEBSOCKET_INCLUDE_DIR ${install_dir}/websocketpp-0.8.2/include)


ExternalProject_Add(
        rapidjson
        PREFIX "vendor/rapidjson"
        GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
        GIT_TAG f54b0e47a08782a6131cc3d60f94d038fa6e0a51
        TIMEOUT 10
        CMAKE_ARGS
        -DRAPIDJSON_BUILD_TESTS=OFF
        -DRAPIDJSON_BUILD_DOC=OFF
        -DRAPIDJSON_BUILD_EXAMPLES=OFF
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
)

ExternalProject_Get_Property(rapidjson source_dir)
set(RAPIDJSON_INCLUDE_DIR ${source_dir}/include)

add_library(libssl SHARED IMPORTED)
set(SSL_LIBS "${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl/libssl.dylib")
set_target_properties(libssl PROPERTIES IMPORTED_LOCATION ${SSL_LIBS})

add_library(libcrypto SHARED IMPORTED)
set(SSL_LIBCRYPTO "${CMAKE_CURRENT_BINARY_DIR}/external/src/openssl/libcrypto.dylib")
set_target_properties(libcrypto PROPERTIES IMPORTED_LOCATION ${SSL_LIBCRYPTO})


#######################################
# Fetch and Build gtest
#######################################

ExternalProject_Add(
        gtest
        DOWNLOAD_COMMAND bash -c "cd ${CMAKE_CURRENT_BINARY_DIR}/lib && tar -xzf ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-release-1.11.0.tar.gz && rm -rf ${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest && mv ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-release-1.11.0  ${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest"
        PREFIX external
        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/../download/googletest
        INSTALL_DIR external/out/gtest
        CONFIGURE_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest && cmake .
        BUILD_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest && make -j4
        INSTALL_COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}
        CMAKE_ARGS
        -DCMAKE_POSITION_INDEPENDENT_CODE=on
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/external/out/gtest
        LOG_DOWNLOAD ON
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir install_dir)

# Create a library targets to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
add_library(libgtest_main IMPORTED STATIC GLOBAL)
add_dependencies(libgtest_main gtest)
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)
add_library(libgmock_main IMPORTED STATIC GLOBAL)
add_dependencies(libgmock_main gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
        "IMPORTED_LOCATION" "${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest/lib/libgtest.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

set_target_properties(libgtest_main PROPERTIES
        "IMPORTED_LOCATION" "${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest/lib/libgtest_main.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

set(GTEST_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/external/src/gtest/googletest/include")
include_directories(${GTEST_INCLUDE_DIR})

#######################################
# Fetch and Build log4cplus
#######################################

ExternalProject_Add(
        log4cplus
        DOWNLOAD_COMMAND bash -c "rm -rf ${CMAKE_CURRENT_BINARY_DIR}/external/src/log4cplus && wget -nc -P ${CMAKE_CURRENT_BINARY_DIR}/../download/log4cplus https://github.com/stardog-union/log4cplus/archive/${LOG4CPLUS_VERSION}.zip && unzip -o ${CMAKE_CURRENT_BINARY_DIR}/../download/log4cplus/${LOG4CPLUS_VERSION}.zip -d ${CMAKE_CURRENT_BINARY_DIR}/external/src && mv ${CMAKE_CURRENT_BINARY_DIR}/external/src/log4cplus-${LOG4CPLUS_VERSION} ${CMAKE_CURRENT_BINARY_DIR}/external/src/log4cplus"
        PREFIX external
        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/../download/log4cplus
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/out/log4cplus
        CMAKE_ARGS
        -DBUILD_SHARED_LIBS=OFF
        -DUNICODE=OFF
        -DLOG4CPLUS_BUILD_LOGGINGSERVER=OFF
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=on
        -DWITH_UNIT_TESTS=OFF
        -DLOG4CPLUS_BUILD_TESTING=OFF
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_C_FLAGS=${DEFAULT_CMAKE_C_FLAGS}
        -DCMAKE_CXX_FLAGS=${DEFAULT_CMAKE_CXX_FLAGS}
        -DCMAKE_C_FLAGS_DEBUG=${DEFAULT_CMAKE_C_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS_DEBUG=${DEFAULT_CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_CXX_FLAGS=-std=c++14
)
ExternalProject_Get_Property(log4cplus source_dir)
set(LOG4CPLUS_INCLUDE_DIR ${source_dir}/include)

add_library(log4cplusS SHARED IMPORTED)
set(LOG4J_LIBS "${CMAKE_CURRENT_BINARY_DIR}/external/src/log4cplus-build/src/liblog4cplusS.a")
set_target_properties(log4cplusS PROPERTIES IMPORTED_LOCATION ${LOG4J_LIBS})

message(STATUS ${BOOST_INCLUDE_DIR})
message(STATUS ${LOG4CPLUS_INCLUDE_DIR})
message(STATUS ${GTEST_INCLUDE_DIR})
message(STATUS ${RAPIDJSON_INCLUDE_DIR})
message(STATUS ${WEBSOCKET_INCLUDE_DIR})
message(STATUS ${OPENSSL_INCLUDE_DIR})

add_dependencies(websocket boost)
add_dependencies(websocket gtest)
add_dependencies(websocket openssl)
add_dependencies(websocket libgtest)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
