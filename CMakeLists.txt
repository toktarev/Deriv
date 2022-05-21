cmake_minimum_required(VERSION 3.15)
project(Derive)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)

set(BOOST_VERSION 1.79.0)
ExternalProject_Add(
        boost
        DOWNLOAD_COMMAND bash -c "rm -rf ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost && wget -nc -P ${CMAKE_CURRENT_BINARY_DIR}/download/boost https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.zip && unzip -o ${CMAKE_CURRENT_BINARY_DIR}/download/boost/boost_1_79_0.zip -d ${CMAKE_CURRENT_BINARY_DIR}/external/src && mv ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost_1_79_0 ${CMAKE_CURRENT_BINARY_DIR}/external/src/boost"
        PREFIX external
        DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/download/boost
        INSTALL_DIR external/out/boost
        CONFIGURE_COMMAND ${BOOST_ENV_FLAGS_STR} ./bootstrap.sh ${BOOST_BOOTSTRAP_FLAGS_STR} --prefix=${CMAKE_CURRENT_BINARY_DIR}/external/out/boost --without-icu --with-libraries=filesystem,program_options,system
        BUILD_COMMAND ${BOOST_ENV_FLAGS_STR} ./b2 ${BOOST_BUILD_FLAGS_STR} -q -o boost_build.out -d3 -j4 --ignore-site-config threading=multi variant=release link=static --prefix=${CMAKE_CURRENT_BINARY_DIR}/external/out/boost --disable-icu --with-filesystem --with-program_options --with-system
        INSTALL_COMMAND ${BOOST_ENV_FLAGS_STR} ./b2 ${BOOST_BUILD_FLAGS_STR}  -q -d3 -j4 --ignore-site-config install
        BUILD_IN_SOURCE 1
        LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(boost install_dir)
set(BOOST_INCLUDE_DIR ${install_dir}/include)

add_library(libboost_asio STATIC IMPORTED)
set(LIBBOOST_ASIO_A ${install_dir}/lib/libboost_asio.a)
set_property(TARGET libboost_asio PROPERTY IMPORTED_LOCATION ${LIBBOOST_ASIO_A})

add_library(libboost_system STATIC IMPORTED)
set(LIBBOOST_SYSTEM_A ${install_dir}/lib/libboost_system.a)
set_property(TARGET libboost_system PROPERTY IMPORTED_LOCATION ${LIBBOOST_SYSTEM_A})

message(STATUS ${install_dir}/include)

add_subdirectory(src)