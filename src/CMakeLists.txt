message(STATUS ${BOOST_INCLUDE_DIR})
include_directories(${BOOST_INCLUDE_DIR})

add_executable(Derive main.cpp)

target_include_directories(Derive PUBLIC ${BOOST_INCLUDE_DIR}/include)

target_link_libraries(Derive ${Boost_LIBRARIES})

add_dependencies(Derive boost)

install(TARGETS Derive DESTINATION ${CMAKE_SOURCE_DIR}/build/native/lib/Derive)
