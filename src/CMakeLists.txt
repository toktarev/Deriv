file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(/usr/local/include)
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${RAPIDJSON_INCLUDE_DIR})
include_directories(${LOG4CPLUS_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(DerivLib ${SOURCES} ${HEADERS})
add_executable(Deriv ${SOURCES} ${CMAKE_SOURCE_DIR}/main.cpp ${HEADERS})

set_target_properties(Deriv PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(DerivLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Deriv PUBLIC /usr/local/include ${BOOST_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${BOOST_INCLUDE_DIR} ${LOG4CPLUS_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
target_include_directories(DerivLib PUBLIC /usr/local/include ${BOOST_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${BOOST_INCLUDE_DIR} ${LOG4CPLUS_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(DerivLib ${Boost_LIBRARIES} libssl libcrypto log4cplusS)

target_link_libraries(Deriv ${Boost_LIBRARIES} libssl libcrypto log4cplusS)

add_dependencies(DerivLib boost)
add_dependencies(DerivLib log4cplus)
add_dependencies(DerivLib rapidjson)
add_dependencies(DerivLib websocket)

add_dependencies(Deriv boost)
add_dependencies(Deriv log4cplus)
add_dependencies(Deriv rapidjson)
add_dependencies(Deriv websocket)

set_target_properties(Deriv  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

install(TARGETS Deriv DESTINATION ${CMAKE_SOURCE_DIR}/Deriv)
