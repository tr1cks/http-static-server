cmake_minimum_required(VERSION 3.2)
project(http_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp headers/common.h src/Connection.cpp headers/Connection.h src/Server.cpp headers/Server.h headers/Request.h headers/Response.h)
add_executable(http_server ${SOURCE_FILES})
target_link_libraries(http_server ${Boost_LIBRARIES})
set_target_properties(http_server PROPERTIES OUTPUT_NAME final)
